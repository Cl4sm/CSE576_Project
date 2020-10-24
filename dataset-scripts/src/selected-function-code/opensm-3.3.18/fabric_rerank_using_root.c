static int fabric_rerank_using_root(IN ftree_fabric_t * p_ftree,
				    IN cl_list_t* p_ranking_bfs_list)
{
	ftree_sw_t *p_sw = NULL;
	ftree_sw_t *p_next_sw;
	int res;

	OSM_LOG_ENTER(&p_ftree->p_osm->log);

	p_next_sw = (ftree_sw_t *) cl_qmap_head(&p_ftree->sw_tbl);
	while (p_next_sw != (ftree_sw_t *) cl_qmap_end(&p_ftree->sw_tbl)) {
		p_sw = p_next_sw;
		p_next_sw = (ftree_sw_t *) cl_qmap_next(&p_sw->map_item);
		if (p_sw->rank == 0)
			cl_list_insert_tail(p_ranking_bfs_list, p_sw);
		else
			p_sw->rank = 0xFFFFFFFF;
	}
	res = fabric_rank_from_roots(p_ftree, p_ranking_bfs_list);
	OSM_LOG_EXIT(&p_ftree->p_osm->log);
	return res;
}