static cl_status_t physp_get_dr_physp_set(IN osm_log_t * p_log,
					  IN osm_subn_t const *p_subn,
					  IN osm_dr_path_t const *p_path,
					  OUT cl_map_t * p_physp_map)
{
	osm_port_t *p_port;
	osm_physp_t *p_physp;
	osm_node_t *p_node;
	uint8_t hop;
	cl_status_t status = CL_SUCCESS;

	OSM_LOG_ENTER(p_log);

	/* find the OSM node */
	p_port = osm_get_port_by_guid(p_subn, p_subn->sm_port_guid);
	if (!p_port) {
		OSM_LOG(p_log, OSM_LOG_ERROR, "ERR 4103: "
			"Failed to find the SM own port by guid\n");
		status = CL_ERROR;
		goto Exit;
	}

	/* get the node of the SM */
	p_node = p_port->p_node;

	/*
	   traverse the path adding the nodes to the table
	   start after the first dummy hop and stop just before the
	   last one
	 */
	for (hop = 1; hop < p_path->hop_count - 1; hop++) {
		/* go out using the phys port of the path */
		p_physp = osm_node_get_physp_ptr(p_node, p_path->path[hop]);

		/* make sure we got a valid port and it has a remote port */
		if (!p_physp) {
			OSM_LOG(p_log, OSM_LOG_ERROR, "ERR 4104: "
				"DR Traversal stopped on invalid port at hop:%u\n",
				hop);
			status = CL_ERROR;
			goto Exit;
		}

		/* we track the ports we go out along the path */
		if (hop > 1)
			cl_map_insert(p_physp_map, ptr_to_key(p_physp), NULL);

		OSM_LOG(p_log, OSM_LOG_DEBUG,
			"Traversed through node: 0x%016" PRIx64
			" port:%u\n",
			cl_ntoh64(p_node->node_info.node_guid),
			p_path->path[hop]);

		if (!(p_physp = osm_physp_get_remote(p_physp))) {
			OSM_LOG(p_log, OSM_LOG_ERROR, "ERR 4106: "
				"DR Traversal stopped on missing remote physp at hop:%u\n",
				hop);
			status = CL_ERROR;
			goto Exit;
		}

		p_node = osm_physp_get_node_ptr(p_physp);
	}

Exit:
	OSM_LOG_EXIT(p_log);
	return status;
}