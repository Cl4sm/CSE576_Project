static void ucast_mgr_process_neighbors(IN cl_map_item_t * p_map_item,
					IN void *context)
{
	osm_switch_t * p_sw = (osm_switch_t *) p_map_item;
	osm_ucast_mgr_t * p_mgr = context;
	osm_node_t *p_node;
	osm_node_t *p_remote_node;
	uint32_t port_num;
	uint8_t remote_port_num;
	uint32_t num_ports;
	osm_physp_t *p_physp;

	OSM_LOG_ENTER(p_mgr->p_log);

	p_node = p_sw->p_node;

	CL_ASSERT(p_node);
	CL_ASSERT(osm_node_get_type(p_node) == IB_NODE_TYPE_SWITCH);

	OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
		"Processing switch with GUID 0x%" PRIx64 "\n",
		cl_ntoh64(osm_node_get_node_guid(p_node)));

	num_ports = osm_node_get_num_physp(p_node);

	/*
	   Start with port 1 to skip the switch's management port.
	 */
	for (port_num = 1; port_num < num_ports; port_num++) {
		p_remote_node = osm_node_get_remote_node(p_node,
							 (uint8_t) port_num,
							 &remote_port_num);
		if (p_remote_node && p_remote_node->sw
		    && (p_remote_node != p_node)) {
			/* make sure the link is healthy. If it is not - don't
			   propagate through it. */
			p_physp = osm_node_get_physp_ptr(p_node, port_num);
			if (!p_physp || !osm_link_is_healthy(p_physp))
				continue;

			ucast_mgr_process_neighbor(p_mgr, p_sw,
						   p_remote_node->sw,
						   (uint8_t) port_num,
						   remote_port_num);
		}
	}

	OSM_LOG_EXIT(p_mgr->p_log);
}