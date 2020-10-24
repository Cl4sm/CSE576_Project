static void rank_switches_from_leafs(IN ftree_fabric_t * p_ftree,
				     IN cl_list_t * p_ranking_bfs_list)
{
	ftree_sw_t *p_sw;
	ftree_sw_t *p_remote_sw;
	osm_node_t *p_node;
	osm_node_t *p_remote_node;
	osm_physp_t *p_osm_port;
	uint8_t i;
	unsigned max_rank = 0;

	while (!cl_is_list_empty(p_ranking_bfs_list)) {
		p_sw = (ftree_sw_t *) cl_list_remove_head(p_ranking_bfs_list);
		p_node = p_sw->p_osm_sw->p_node;

		/* note: skipping port 0 on switches */
		for (i = 1; i < osm_node_get_num_physp(p_node); i++) {
			p_osm_port = osm_node_get_physp_ptr(p_node, i);
			if (!p_osm_port || !osm_link_is_healthy(p_osm_port))
				continue;

			p_remote_node =
			    osm_node_get_remote_node(p_node, i, NULL);
			if (!p_remote_node)
				continue;
			if (osm_node_get_type(p_remote_node) !=
			    IB_NODE_TYPE_SWITCH)
				continue;

			p_remote_sw = fabric_get_sw_by_guid(p_ftree,
							    osm_node_get_node_guid
							    (p_remote_node));
			if (!p_remote_sw) {
				/* remote node is not a switch */
				continue;
			}

			/* if needed, rank the remote switch and add it to the BFS list */
			if (sw_update_rank(p_remote_sw, p_sw->rank + 1)) {
				max_rank = p_remote_sw->rank;
				cl_list_insert_tail(p_ranking_bfs_list,
						    p_remote_sw);
			}
		}
	}

	/* set FatTree maximal switch rank */
	p_ftree->max_switch_rank = max_rank;

}				/* rank_switches_from_leafs() */