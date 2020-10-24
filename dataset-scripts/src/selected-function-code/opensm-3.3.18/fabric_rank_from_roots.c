static int fabric_rank_from_roots(IN ftree_fabric_t * p_ftree,
				  IN cl_list_t* p_ranking_bfs_list)
{
	osm_node_t *p_osm_node;
	osm_node_t *p_remote_osm_node;
	osm_physp_t *p_osm_physp;
	ftree_sw_t *p_sw;
	ftree_sw_t *p_remote_sw;
	int res = 0;
	unsigned max_rank = 0;
	unsigned i;

	OSM_LOG_ENTER(&p_ftree->p_osm->log);

	if (!p_ranking_bfs_list) {
		res = -1;
		goto Exit;
	}
	while (!cl_is_list_empty(p_ranking_bfs_list)) {
		p_sw = (ftree_sw_t *) cl_list_remove_head(p_ranking_bfs_list);
		p_osm_node = p_sw->p_osm_sw->p_node;

		/* note: skipping port 0 on switches */
		for (i = 1; i < osm_node_get_num_physp(p_osm_node); i++) {
			p_osm_physp = osm_node_get_physp_ptr(p_osm_node, i);
			if (!p_osm_physp || !osm_link_is_healthy(p_osm_physp))
				continue;

			p_remote_osm_node =
			    osm_node_get_remote_node(p_osm_node, i, NULL);
			if (!p_remote_osm_node)
				continue;

			if (osm_node_get_type(p_remote_osm_node) !=
			    IB_NODE_TYPE_SWITCH)
				continue;

			p_remote_sw = fabric_get_sw_by_guid(p_ftree,
							    osm_node_get_node_guid
							    (p_remote_osm_node));
			CL_ASSERT(p_remote_sw);

			/* if needed, rank the remote switch and add it to the BFS list */
			if (sw_update_rank(p_remote_sw, p_sw->rank + 1)) {
				OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_DEBUG,
					"Ranking switch 0x%" PRIx64
					" with rank %u\n",
					sw_get_guid_ho(p_remote_sw),
					p_remote_sw->rank);
				max_rank = p_remote_sw->rank;
				cl_list_insert_tail(p_ranking_bfs_list,
						    p_remote_sw);
			}
		}
		/* done with ports of this switch - go to the next switch in the list */
	}

	OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_VERBOSE,
		"Subnet ranking completed. Max Node Rank = %u\n", max_rank);

	/* set FatTree maximal switch rank */
	p_ftree->max_switch_rank = max_rank;

Exit:
	OSM_LOG_EXIT(&p_ftree->p_osm->log);
	return res;
}				/* fabric_rank_from_roots() */