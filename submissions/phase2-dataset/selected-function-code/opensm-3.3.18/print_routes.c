static void print_routes(osm_ucast_mgr_t * p_mgr, vertex_t * adj_list,
			 uint32_t adj_list_size, osm_port_t * port)
{
	uint32_t i = 0, j = 0;

	for (i = 1; i < adj_list_size; i++) {
		if (adj_list[i].state == DISCOVERED) {
			OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
				"Route from 0x%" PRIx64 " (%s) to 0x%" PRIx64
				" (%s):\n", adj_list[i].guid,
				adj_list[i].sw->p_node->print_desc,
				cl_ntoh64(osm_node_get_node_guid(port->p_node)),
				port->p_node->print_desc);
			j = i;
			while (adj_list[j].used_link) {
				if (j > 0) {
					OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
						"   0x%" PRIx64
						" (%s) routes thru port %" PRIu8
						"\n", adj_list[j].guid,
						adj_list[j].sw->p_node->
						print_desc,
						adj_list[j].used_link->to_port);
				} else {
					OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
						"   0x%" PRIx64
						" (%s) routes thru port %" PRIu8
						"\n", adj_list[j].guid,
						port->p_node->print_desc,
						adj_list[j].used_link->to_port);
				}
				j = adj_list[j].used_link->from;
			}
		}
	}
}