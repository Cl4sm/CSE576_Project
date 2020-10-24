static void dfsssp_print_graph(osm_ucast_mgr_t * p_mgr, vertex_t * adj_list,
			       uint32_t size)
{
	uint32_t i = 0, c = 0;
	link_t *link = NULL;

	/* index 0 is for the source in dijkstra -> ignore */
	for (i = 1; i < size; i++) {
		OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG, "adj_list[%" PRIu32 "]:\n",
			i);
		OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
			"   guid = 0x%" PRIx64 " lid = %" PRIu16 " (%s)\n",
			adj_list[i].guid, adj_list[i].lid,
			adj_list[i].sw->p_node->print_desc);
		OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
			"   num_hca = %" PRIu32 "\n", adj_list[i].num_hca);

		c = 1;
		for (link = adj_list[i].links; link != NULL;
		     link = link->next, c++) {
			OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
				"   link[%" PRIu32 "]:\n", c);
			OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
				"      to guid = 0x%" PRIx64 " (%s) port %"
				PRIu8 "\n", link->guid,
				adj_list[link->to].sw->p_node->print_desc,
				link->to_port);
			OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
				"      weight on this link = %" PRIu64 "\n",
				link->weight);
		}
	}
}