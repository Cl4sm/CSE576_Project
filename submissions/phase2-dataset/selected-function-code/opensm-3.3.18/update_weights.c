static void update_weights(osm_ucast_mgr_t * p_mgr, vertex_t * adj_list,
			   uint32_t adj_list_size)
{
	uint32_t i = 0, j = 0;
	uint32_t additional_weight = 0;

	OSM_LOG_ENTER(p_mgr->p_log);

	for (i = 1; i < adj_list_size; i++) {
		/* if no route goes thru this switch -> cycle */
		if (!(adj_list[i].used_link))
			continue;
		additional_weight = adj_list[i].num_hca;

		j = i;
		while (adj_list[j].used_link) {
			/* update the link from pre to this node */
			adj_list[j].used_link->weight += additional_weight;

			j = adj_list[j].used_link->from;
		}
	}

	OSM_LOG_EXIT(p_mgr->p_log);
}