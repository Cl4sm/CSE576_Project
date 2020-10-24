void osm_drop_mgr_process(osm_sm_t * sm)
{
	cl_qmap_t *p_node_guid_tbl, *p_port_guid_tbl;
	osm_port_t *p_port, *p_next_port;
	osm_node_t *p_node, *p_next_node;
	int max_ports, port_num;
	osm_physp_t *p_physp;
	ib_net64_t port_guid;

	CL_ASSERT(sm);

	OSM_LOG_ENTER(sm->p_log);

	p_node_guid_tbl = &sm->p_subn->node_guid_tbl;
	p_port_guid_tbl = &sm->p_subn->port_guid_tbl;

	CL_PLOCK_EXCL_ACQUIRE(sm->p_lock);

	p_next_node = (osm_node_t *) cl_qmap_head(p_node_guid_tbl);
	while (p_next_node != (osm_node_t *) cl_qmap_end(p_node_guid_tbl)) {
		p_node = p_next_node;
		p_next_node =
		    (osm_node_t *) cl_qmap_next(&p_next_node->map_item);

		CL_ASSERT(cl_qmap_key(&p_node->map_item) ==
			  osm_node_get_node_guid(p_node));

		OSM_LOG(sm->p_log, OSM_LOG_DEBUG,
			"Checking node 0x%016" PRIx64 "\n",
			cl_ntoh64(osm_node_get_node_guid(p_node)));

		/*
		   Check if this node was discovered during the last sweep.
		   If not, it is unreachable in the current subnet, and
		   should therefore be removed from the subnet object.
		 */
		if (p_node->discovery_count == 0)
			drop_mgr_process_node(sm, p_node);
		else {
			/*
			 * We want to preserve the configured pkey indexes,
			 * so if we don't receive GetResp P_KeyTable for some block,
			 * do the following:
			 *   1. Drop node if the node is sw and got timeout for port 0.
			 *   2. Drop node if node is HCA/RTR.
			 *   3. Drop only physp if got timeout for sw when the port isn't 0.
			 * We'll set error during initialization in order to
			 * cause an immediate heavy sweep and try to get the
			 * configured P_KeyTable again.
			 */
			if (osm_node_get_type(p_node) == IB_NODE_TYPE_SWITCH)
				port_num = 0;
			else
				port_num = 1;
			max_ports = osm_node_get_num_physp(p_node);
			for (; port_num < max_ports; port_num++) {
				p_physp = osm_node_get_physp_ptr(p_node, port_num);
				if (!p_physp || p_physp->pkeys.rcv_blocks_cnt == 0)
					continue;
				p_physp->pkeys.rcv_blocks_cnt = 0;
				p_physp->need_update = 2;
				sm->p_subn->subnet_initialization_error = TRUE;
				port_guid = osm_physp_get_port_guid(p_physp);
				p_port = osm_get_port_by_guid(sm->p_subn, port_guid);
				CL_ASSERT(p_port);
				if (p_node->physp_discovered[port_num]) {
					p_node->physp_discovered[port_num] = 0;
					p_port->discovery_count--;
				}
			}
		}
	}

	/*
	   Go over all the nodes. If the node is a switch - make sure
	   there is also a switch record for it, and a portInfo record for
	   port zero of of the node.
	   If not - this means that there was some error in getting the data
	   of this node. Drop the node.
	 */
	p_next_node = (osm_node_t *) cl_qmap_head(p_node_guid_tbl);
	while (p_next_node != (osm_node_t *) cl_qmap_end(p_node_guid_tbl)) {
		p_node = p_next_node;
		p_next_node =
		    (osm_node_t *) cl_qmap_next(&p_next_node->map_item);

		OSM_LOG(sm->p_log, OSM_LOG_DEBUG,
			"Checking full discovery of node 0x%016" PRIx64 "\n",
			cl_ntoh64(osm_node_get_node_guid(p_node)));

		if (osm_node_get_type(p_node) != IB_NODE_TYPE_SWITCH)
			continue;

		/* We are handling a switch node */
		drop_mgr_check_node(sm, p_node);
	}

	p_next_port = (osm_port_t *) cl_qmap_head(p_port_guid_tbl);
	while (p_next_port != (osm_port_t *) cl_qmap_end(p_port_guid_tbl)) {
		p_port = p_next_port;
		p_next_port =
		    (osm_port_t *) cl_qmap_next(&p_next_port->map_item);

		CL_ASSERT(cl_qmap_key(&p_port->map_item) ==
			  osm_port_get_guid(p_port));

		OSM_LOG(sm->p_log, OSM_LOG_DEBUG,
			"Checking port 0x%016" PRIx64 "\n",
			cl_ntoh64(osm_port_get_guid(p_port)));

		/*
		   If the port is unreachable, remove it from the guid table.
		 */
		if (p_port->discovery_count == 0)
			drop_mgr_remove_port(sm, p_port);
	}

	CL_PLOCK_RELEASE(sm->p_lock);
	OSM_LOG_EXIT(sm->p_log);
}