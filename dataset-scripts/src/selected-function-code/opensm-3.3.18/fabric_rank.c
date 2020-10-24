static int fabric_rank(IN ftree_fabric_t * p_ftree)
{
	int res = -1;
	cl_list_t ranking_bfs_list;

	OSM_LOG_ENTER(&p_ftree->p_osm->log);
	cl_list_init(&ranking_bfs_list, 10);

	if (fabric_roots_provided(p_ftree) &&
	    fabric_load_roots(p_ftree, &ranking_bfs_list))
		res = fabric_rank_from_roots(p_ftree, &ranking_bfs_list);
	else {
		res = fabric_rank_from_hcas(p_ftree);
		if (!res)
			res = fabric_rerank_using_root(p_ftree, &ranking_bfs_list);
	}

	if (res)
		goto Exit;

	OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_VERBOSE,
		"FatTree max switch rank is %u\n", p_ftree->max_switch_rank);

Exit:
	cl_list_destroy(&ranking_bfs_list);
	OSM_LOG_EXIT(&p_ftree->p_osm->log);
	return res;
}				/* fabric_rank() */