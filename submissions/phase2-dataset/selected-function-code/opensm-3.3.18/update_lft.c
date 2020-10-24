static int update_lft(osm_ucast_mgr_t * p_mgr, vertex_t * adj_list,
		      uint32_t adj_list_size, osm_port_t * p_port, uint16_t lid)
{
	uint32_t i = 0;
	uint8_t port = 0;
	uint8_t hops = 0;
	osm_switch_t *p_sw = NULL;
	boolean_t is_ignored_by_port_prof = FALSE;
	osm_physp_t *p = NULL;
	cl_status_t ret;

	OSM_LOG_ENTER(p_mgr->p_log);

	for (i = 1; i < adj_list_size; i++) {
		/* if no route goes thru this switch -> cycle */
		if (!(adj_list[i].used_link))
			continue;

		p_sw = adj_list[i].sw;
		hops = adj_list[i].hops;
		port = adj_list[i].used_link->to_port;
		/* the used_link is the link that was used in dijkstra to reach this node,
		   so the to_port is the local port on this node
		 */

		if (port == OSM_NO_PATH) {	/* if clause shouldn't be possible in this routing, but who cares */
			OSM_LOG(p_mgr->p_log, OSM_LOG_ERROR,
				"ERR AD06: No path to get to LID %" PRIu16
				" from switch 0x%" PRIx64 "\n", lid,
				cl_ntoh64(osm_node_get_node_guid
					  (p_sw->p_node)));

			/* do not try to overwrite the ppro of non existing port ... */
			is_ignored_by_port_prof = TRUE;
			return 1;
		} else {
			OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
				"Routing LID %" PRIu16 " to port %" PRIu8
				" for switch 0x%" PRIx64 "\n", lid, port,
				cl_ntoh64(osm_node_get_node_guid
					  (p_sw->p_node)));

			p = osm_node_get_physp_ptr(p_sw->p_node, port);

			/* we would like to optionally ignore this port in equalization
			   as in the case of the Mellanox Anafa Internal PCI TCA port
			 */
			is_ignored_by_port_prof = p->is_prof_ignored;

			/* We also would ignore this route if the target lid is of
			   a switch and the port_profile_switch_node is not TRUE
			 */
			if (!p_mgr->p_subn->opt.port_profile_switch_nodes)
				is_ignored_by_port_prof |=
				    (osm_node_get_type(p_port->p_node) ==
				     IB_NODE_TYPE_SWITCH);
		}

		/* to support lmc > 0 the functions alloc_ports_priv, free_ports_priv, find_and_add_remote_sys
		   from minhop aren't needed cause osm_switch_recommend_path is implicit calulated
		   for each LID pair thru dijkstra;
		   for each port the dijkstra algorithm calculates (max_lid_ho - min_lid_ho)-times maybe
		   disjoint routes to spread the bandwidth -> diffent routes for one port and lmc>0
		 */

		/* set port in LFT */
		p_sw->new_lft[lid] = port;
		if (!is_ignored_by_port_prof) {
			/* update the number of path routing thru this port */
			osm_switch_count_path(p_sw, port);
		}
		/* set te hop count from this switch to the lid */
		ret = osm_switch_set_hops(p_sw, lid, port, hops);
		if (ret != CL_SUCCESS)
			OSM_LOG(p_mgr->p_log, OSM_LOG_ERROR,
				"ERR AD05: cannot set hops for LID %" PRIu16
				" at switch 0x%" PRIx64 "\n", lid,
				cl_ntoh64(osm_node_get_node_guid
					  (p_sw->p_node)));
	}

	OSM_LOG_EXIT(p_mgr->p_log);
	return 0;
}