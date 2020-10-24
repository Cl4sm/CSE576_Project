static int fabric_rank_from_hcas(IN ftree_fabric_t * p_ftree)
{
	ftree_hca_t *p_hca;
	ftree_hca_t *p_next_hca;
	cl_list_t ranking_bfs_list;
	int res = 0;

	OSM_LOG_ENTER(&p_ftree->p_osm->log);

	cl_list_init(&ranking_bfs_list, 10);

	/* Mark REVERSED rank of all the switches in the subnet.
	   Start from switches that are connected to hca's, and
	   scan all the switches in the subnet. */
	p_next_hca = (ftree_hca_t *) cl_qmap_head(&p_ftree->hca_tbl);
	while (p_next_hca != (ftree_hca_t *) cl_qmap_end(&p_ftree->hca_tbl)) {
		p_hca = p_next_hca;
		p_next_hca = (ftree_hca_t *) cl_qmap_next(&p_hca->map_item);
		if (rank_leaf_switches(p_ftree, p_hca, &ranking_bfs_list) != 0) {
			res = -1;
			OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_ERROR,
				"ERR AB14: "
				"Subnet ranking failed - subnet is not FatTree");
			goto Exit;
		}
	}

	/* Now rank rest of the switches in the fabric, while the
	   list already contains all the ranked leaf switches */
	rank_switches_from_leafs(p_ftree, &ranking_bfs_list);

	/* fix ranking of the switches by reversing the ranking direction */
	cl_qmap_apply_func(&p_ftree->sw_tbl, sw_reverse_rank, (void *)p_ftree);

Exit:
	cl_list_destroy(&ranking_bfs_list);
	OSM_LOG_EXIT(&p_ftree->p_osm->log);
	return res;
}				/* fabric_rank_from_hcas() */