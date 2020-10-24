static void drop_mgr_check_node(osm_sm_t * sm, IN osm_node_t * p_node)
{
	ib_net64_t node_guid;
	osm_physp_t *p_physp, *p_remote_physp;
	osm_node_t *p_remote_node;
	osm_port_t *p_port;
	ib_net64_t port_guid;
	uint8_t port_num, remote_port_num;

	OSM_LOG_ENTER(sm->p_log);

	node_guid = osm_node_get_node_guid(p_node);

	if (osm_node_get_type(p_node) != IB_NODE_TYPE_SWITCH) {
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 0107: "
			"Node 0x%016" PRIx64 " is not a switch node\n",
			cl_ntoh64(node_guid));
		goto Exit;
	}

	/* Make sure we have a switch object for this node */
	if (!p_node->sw) {
		/* We do not have switch info for this node */
		OSM_LOG(sm->p_log, OSM_LOG_VERBOSE,
			"Node 0x%016" PRIx64 " no switch in table\n",
			cl_ntoh64(node_guid));

		drop_mgr_process_node(sm, p_node);
		goto Exit;
	}

	/* Make sure we have a port object for port zero */
	p_physp = osm_node_get_physp_ptr(p_node, 0);
	if (!p_physp) {
		OSM_LOG(sm->p_log, OSM_LOG_VERBOSE,
			"Node 0x%016" PRIx64 " no valid physical port 0\n",
			cl_ntoh64(node_guid));

		drop_mgr_process_node(sm, p_node);
		goto Exit;
	}

	port_guid = osm_physp_get_port_guid(p_physp);

	p_port = osm_get_port_by_guid(sm->p_subn, port_guid);

	if (!p_port) {
		OSM_LOG(sm->p_log, OSM_LOG_VERBOSE,
			"Node 0x%016" PRIx64 " has no port object\n",
			cl_ntoh64(node_guid));

		drop_mgr_process_node(sm, p_node);
		goto Exit;
	}

	if (!p_node->physp_discovered[0]) {
		OSM_LOG(sm->p_log, OSM_LOG_VERBOSE,
			"Node 0x%016" PRIx64 " port has discovery count zero\n",
			cl_ntoh64(node_guid));

		drop_mgr_process_node(sm, p_node);
		goto Exit;
	}

	/*
	 * Unlink all ports that havn't been discovered during the last sweep.
	 * Optimization: Skip the check if discovered all the ports of the switch.
	 */
	if (p_port->discovery_count < p_node->physp_tbl_size) {
		for (port_num = 1; port_num < p_node->physp_tbl_size; port_num++) {
			if (!p_node->physp_discovered[port_num]) {
				p_physp = osm_node_get_physp_ptr(p_node, port_num);
				if (!p_physp)
					continue;
				p_remote_physp = osm_physp_get_remote(p_physp);
				if (!p_remote_physp)
					continue;

				p_remote_node =
				    osm_physp_get_node_ptr(p_remote_physp);
				remote_port_num =
				    osm_physp_get_port_num(p_remote_physp);

				OSM_LOG(sm->p_log, OSM_LOG_VERBOSE,
					"Unlinking local node 0x%" PRIx64
					", port %u"
					"\n\t\t\t\tand remote node 0x%" PRIx64
					", port %u due to missing PortInfo\n",
					cl_ntoh64(osm_node_get_node_guid
						  (p_node)), port_num,
					cl_ntoh64(osm_node_get_node_guid
						  (p_remote_node)),
					remote_port_num);

				if (sm->ucast_mgr.cache_valid)
					osm_ucast_cache_add_link(&sm->ucast_mgr,
								 p_physp,
								 p_remote_physp);

				osm_node_unlink(p_node, (uint8_t) port_num,
						p_remote_node,
						(uint8_t) remote_port_num);
			}
		}
	}
Exit:
	OSM_LOG_EXIT(sm->p_log);
	return;
}