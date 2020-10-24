static boolean_t drop_mgr_process_node(osm_sm_t * sm, IN osm_node_t * p_node)
{
	osm_physp_t *p_physp;
	osm_port_t *p_port;
	osm_node_t *p_node_check;
	uint32_t port_num;
	uint32_t max_ports;
	ib_net64_t port_guid;
	boolean_t return_val = FALSE;

	OSM_LOG_ENTER(sm->p_log);

	OSM_LOG(sm->p_log, OSM_LOG_VERBOSE,
		"Unreachable node 0x%016" PRIx64 "\n",
		cl_ntoh64(osm_node_get_node_guid(p_node)));

	if (sm->ucast_mgr.cache_valid)
		osm_ucast_cache_add_node(&sm->ucast_mgr, p_node);

	/*
	   Delete all the logical and physical port objects
	   associated with this node.
	 */
	max_ports = osm_node_get_num_physp(p_node);
	for (port_num = 0; port_num < max_ports; port_num++) {
		p_physp = osm_node_get_physp_ptr(p_node, port_num);
		if (p_physp) {
			port_guid = osm_physp_get_port_guid(p_physp);

			p_port = osm_get_port_by_guid(sm->p_subn, port_guid);

			if (p_port)
				drop_mgr_remove_port(sm, p_port);
			else
				drop_mgr_clean_physp(sm, p_physp);
		}
	}

	return_val = TRUE;

	if (p_node->sw)
		drop_mgr_remove_switch(sm, p_node);

	p_node_check =
	    (osm_node_t *) cl_qmap_remove(&sm->p_subn->node_guid_tbl,
					  osm_node_get_node_guid(p_node));
	if (p_node_check != p_node) {
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 0105: "
			"Node 0x%016" PRIx64 " not in guid table\n",
			cl_ntoh64(osm_node_get_node_guid(p_node)));
	}

	/* free memory allocated to node */
	osm_node_delete(&p_node);

	OSM_LOG_EXIT(sm->p_log);
	return return_val;
}