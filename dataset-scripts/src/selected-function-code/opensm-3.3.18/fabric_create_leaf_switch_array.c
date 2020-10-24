static int fabric_create_leaf_switch_array(IN ftree_fabric_t * p_ftree)
{
	ftree_sw_t *p_sw;
	ftree_sw_t *p_next_sw;
	ftree_sw_t **all_switches_at_leaf_level;
	unsigned i;
	unsigned all_leaf_idx = 0;
	unsigned first_leaf_idx;
	unsigned last_leaf_idx;
	int res = 0;

	OSM_LOG_ENTER(&p_ftree->p_osm->log);

	/* create array of ALL the switches that have leaf rank */
	all_switches_at_leaf_level = (ftree_sw_t **)
	    malloc(cl_qmap_count(&p_ftree->sw_tbl) * sizeof(ftree_sw_t *));
	if (!all_switches_at_leaf_level) {
		osm_log_v2(&p_ftree->p_osm->log, OSM_LOG_SYS, FILE_ID,
			   "Fat-tree routing: Memory allocation failed\n");
		res = -1;
		goto Exit;
	}
	memset(all_switches_at_leaf_level, 0,
	       cl_qmap_count(&p_ftree->sw_tbl) * sizeof(ftree_sw_t *));

	p_next_sw = (ftree_sw_t *) cl_qmap_head(&p_ftree->sw_tbl);
	while (p_next_sw != (ftree_sw_t *) cl_qmap_end(&p_ftree->sw_tbl)) {
		p_sw = p_next_sw;
		p_next_sw = (ftree_sw_t *) cl_qmap_next(&p_sw->map_item);
		if (p_sw->rank == p_ftree->leaf_switch_rank) {
			OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_DEBUG,
				"Adding switch 0x%" PRIx64
				" to full leaf switch array\n",
				sw_get_guid_ho(p_sw));
			all_switches_at_leaf_level[all_leaf_idx++] = p_sw;
		}
	}

	/* quick-sort array of leaf switches by index */
	qsort(all_switches_at_leaf_level,	/* array */
	      all_leaf_idx,	/* number of elements */
	      sizeof(ftree_sw_t *),	/* size of each element */
	      compare_switches_by_index);	/* comparator */

	/* check the first and the last REAL leaf (the one
	   that has CNs) in the array of all the leafs */

	first_leaf_idx = all_leaf_idx;
	last_leaf_idx = 0;
	for (i = 0; i < all_leaf_idx; i++) {
		if (all_switches_at_leaf_level[i]->is_leaf) {
			if (i < first_leaf_idx)
				first_leaf_idx = i;
			last_leaf_idx = i;
		}
	}

	OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_DEBUG,
		"Full leaf array info: first_leaf_idx = %u, last_leaf_idx = %u\n",
		first_leaf_idx, last_leaf_idx);

	if (first_leaf_idx >= last_leaf_idx) {
		osm_log_v2(&p_ftree->p_osm->log, OSM_LOG_INFO, FILE_ID,
			   "Faild to find leaf switches - topology is not "
			   "fat-tree\n");
		res = -1;
		goto Exit;
	}

	/* Create array of REAL leaf switches, sorted by index.
	   This array may contain switches at the same rank w/o CNs,
	   in case this is the order of indexing. */
	p_ftree->leaf_switches_num = last_leaf_idx - first_leaf_idx + 1;
	p_ftree->leaf_switches = (ftree_sw_t **)
	    malloc(p_ftree->leaf_switches_num * sizeof(ftree_sw_t *));
	if (!p_ftree->leaf_switches) {
		osm_log_v2(&p_ftree->p_osm->log, OSM_LOG_SYS, FILE_ID,
			   "Fat-tree routing: Memory allocation failed\n");
		res = -1;
		goto Exit;
	}

	memcpy(p_ftree->leaf_switches,
	       &(all_switches_at_leaf_level[first_leaf_idx]),
	       p_ftree->leaf_switches_num * sizeof(ftree_sw_t *));

	OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_DEBUG,
		"Created array of %u leaf switches\n",
		p_ftree->leaf_switches_num);

Exit:
	free(all_switches_at_leaf_level);
	OSM_LOG_EXIT(&p_ftree->p_osm->log);
	return res;
}				/* fabric_create_leaf_switch_array() */