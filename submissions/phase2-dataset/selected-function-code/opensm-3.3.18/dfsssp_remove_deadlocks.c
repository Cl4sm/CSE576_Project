static int dfsssp_remove_deadlocks(dfsssp_context_t * dfsssp_ctx)
{
	osm_ucast_mgr_t *p_mgr = (osm_ucast_mgr_t *) dfsssp_ctx->p_mgr;

	cl_qlist_t *port_tbl = &p_mgr->port_order_list;	/* 1 management port per switch + 1 or 2 ports for each Hca */
	cl_list_item_t *item1 = NULL, *item2 = NULL;
	osm_port_t *src_port = NULL, *dest_port = NULL;

	uint32_t i = 0, j = 0, err = 0;
	uint8_t vl = 0, test_vl = 0, vl_avail = 0, vl_needed = 1;
	double most_avg_paths = 0.0;
	cdg_node_t **cdg = NULL, *start_here = NULL, *cycle = NULL;
	cdg_link_t *weakest_link = NULL;
	uint32_t srcdest = 0;

	vltable_t *srcdest2vl_table = NULL;
	uint8_t lmc = 0;
	uint16_t slid = 0, dlid = 0, min_lid_ho = 0, max_lid_ho =
	    0, min_lid_ho2 = 0, max_lid_ho2 = 0;;
	uint64_t *paths_per_vl = NULL;
	uint64_t from = 0, to = 0, count = 0;
	uint8_t *split_count = NULL;
	uint8_t ntype = 0;

	OSM_LOG_ENTER(p_mgr->p_log);
	OSM_LOG(p_mgr->p_log, OSM_LOG_VERBOSE,
		"Assign each src/dest pair a Virtual Lanes, to remove deadlocks in the routing\n");

	vl_avail = get_avail_vl_in_subn(p_mgr);
	OSM_LOG(p_mgr->p_log, OSM_LOG_INFO,
		"Virtual Lanes available: %" PRIu8 "\n", vl_avail);

	paths_per_vl = (uint64_t *) malloc(vl_avail * sizeof(uint64_t));
	if (!paths_per_vl) {
		OSM_LOG(p_mgr->p_log, OSM_LOG_ERROR,
			"ERR AD22: cannot allocate memory for paths_per_vl\n");
		return 1;
	}
	memset(paths_per_vl, 0, vl_avail * sizeof(uint64_t));

	cdg = (cdg_node_t **) malloc(vl_avail * sizeof(cdg_node_t *));
	if (!cdg) {
		OSM_LOG(p_mgr->p_log, OSM_LOG_ERROR,
			"ERR AD23: cannot allocate memory for cdg\n");
		free(paths_per_vl);
		return 1;
	}
	for (i = 0; i < vl_avail; i++)
		cdg[i] = NULL;

	count = 0;
	/* count all ports (also multiple LIDs) of type CA or SP0 for size of VL table */
	for (item1 = cl_qlist_head(port_tbl); item1 != cl_qlist_end(port_tbl);
	     item1 = cl_qlist_next(item1)) {
		dest_port = (osm_port_t *)cl_item_obj(item1, dest_port,
						      list_item);
		ntype = osm_node_get_type(dest_port->p_node);
		if (ntype == IB_NODE_TYPE_CA || ntype == IB_NODE_TYPE_SWITCH) {
			/* only SP0 with SLtoVLMapping support will be processed */
			if (ntype == IB_NODE_TYPE_SWITCH
			    && !(dest_port->p_physp->port_info.capability_mask
			    & IB_PORT_CAP_HAS_SL_MAP))
				continue;

			lmc = osm_port_get_lmc(dest_port);
			count += (1 << lmc);
		}
	}
	/* allocate VL table and indexing array */
	err = vltable_alloc(&srcdest2vl_table, count);
	if (err) {
		OSM_LOG(p_mgr->p_log, OSM_LOG_ERROR,
			"ERR AD26: cannot allocate memory for srcdest2vl_table\n");
		goto ERROR;
	}

	i = 0;
	/* fill lids into indexing array */
	for (item1 = cl_qlist_head(port_tbl); item1 != cl_qlist_end(port_tbl);
	     item1 = cl_qlist_next(item1)) {
		dest_port = (osm_port_t *)cl_item_obj(item1, dest_port,
						      list_item);
		ntype = osm_node_get_type(dest_port->p_node);
		if (ntype == IB_NODE_TYPE_CA || ntype == IB_NODE_TYPE_SWITCH) {
			/* only SP0 with SLtoVLMapping support will be processed */
			if (ntype == IB_NODE_TYPE_SWITCH
			    && !(dest_port->p_physp->port_info.capability_mask
			    & IB_PORT_CAP_HAS_SL_MAP))
				continue;

			osm_port_get_lid_range_ho(dest_port, &min_lid_ho,
						  &max_lid_ho);
			for (dlid = min_lid_ho; dlid <= max_lid_ho; dlid++, i++)
				srcdest2vl_table->lids[i] = cl_hton16(dlid);
		}
	}
	/* sort lids */
	vltable_sort_lids(srcdest2vl_table);

	test_vl = 0;
	/* fill cdg[0] with routes from each src/dest port combination for all Hca/SP0 in the subnet */
	for (item1 = cl_qlist_head(port_tbl); item1 != cl_qlist_end(port_tbl);
	     item1 = cl_qlist_next(item1)) {
		dest_port = (osm_port_t *)cl_item_obj(item1, dest_port,
						      list_item);
		ntype = osm_node_get_type(dest_port->p_node);
		if ((ntype != IB_NODE_TYPE_CA && ntype != IB_NODE_TYPE_SWITCH)
		    || !(dest_port->p_physp->port_info.capability_mask
		    & IB_PORT_CAP_HAS_SL_MAP))
			continue;

		for (item2 = cl_qlist_head(port_tbl);
		     item2 != cl_qlist_end(port_tbl);
		     item2 = cl_qlist_next(item2)) {
			src_port = (osm_port_t *)cl_item_obj(item2, src_port,
							     list_item);
			ntype = osm_node_get_type(src_port->p_node);
			if ((ntype != IB_NODE_TYPE_CA
			    && ntype != IB_NODE_TYPE_SWITCH)
			    || !(src_port->p_physp->port_info.capability_mask
			    & IB_PORT_CAP_HAS_SL_MAP))
				continue;

			if (src_port != dest_port) {
				/* iterate over LIDs of src and dest port */
				osm_port_get_lid_range_ho(src_port, &min_lid_ho,
							  &max_lid_ho);
				for (slid = min_lid_ho; slid <= max_lid_ho;
				     slid++) {
					osm_port_get_lid_range_ho
					    (dest_port, &min_lid_ho2,
					     &max_lid_ho2);
					for (dlid = min_lid_ho2;
					     dlid <= max_lid_ho2;
					     dlid++) {

						/* try to add the path to cdg[0] */
						err =
						    update_channel_dep_graph
						    (&(cdg[test_vl]),
						     src_port, slid,
						     dest_port, dlid);
						if (err) {
							OSM_LOG(p_mgr->
								p_log,
								OSM_LOG_ERROR,
								"ERR AD14: cannot allocate memory for cdg node or link in update_channel_dep_graph(...)\n");
							goto ERROR;
						}
						/* add the <s,d> kombination / coresponding virtual lane to the VL table */
						vltable_insert
						    (srcdest2vl_table,
						     cl_hton16(slid),
						     cl_hton16(dlid),
						     test_vl);
						paths_per_vl[test_vl]++;

					}

				}
			}

		}
	}
	dfsssp_ctx->srcdest2vl_table = srcdest2vl_table;

	/* test all cdg for cycles and break the cycles by moving paths on the weakest link to the next cdg */
	for (test_vl = 0; test_vl < vl_avail - 1; test_vl++) {
		start_here = cdg[test_vl];
		while (start_here) {
			cycle =
			    search_cycle_in_channel_dep_graph(cdg[test_vl],
							      start_here);

			if (cycle) {
				vl_needed = test_vl + 2;

				/* calc weakest link n cycle */
				weakest_link = get_weakest_link_in_cycle(cycle);
				if (!weakest_link) {
					OSM_LOG(p_mgr->p_log, OSM_LOG_ERROR,
						"ERR AD27: something went wrong in get_weakest_link_in_cycle(...)\n");
					err = 1;
					goto ERROR;
				}

				paths_per_vl[test_vl] -=
				    weakest_link->num_pairs;
				paths_per_vl[test_vl + 1] +=
				    weakest_link->num_pairs;

				/* move all <s,d> paths on this link to the next cdg */
				for (i = 0; i < weakest_link->num_pairs; i++) {
					srcdest =
					    get_next_srcdest_pair(weakest_link,
								  i);
					slid = (uint16_t) (srcdest >> 16);
					dlid =
					    (uint16_t) ((srcdest << 16) >> 16);

					/* only move if not moved in a previous step */
					if (test_vl !=
					    (uint8_t)
					    vltable_get_vl(srcdest2vl_table,
							   cl_hton16(slid),
							   cl_hton16(dlid))) {
						/* this path has been moved
						   before -> don't count
						 */
						paths_per_vl[test_vl]++;
						paths_per_vl[test_vl + 1]--;
						continue;
					}

					src_port =
					    osm_get_port_by_lid(p_mgr->p_subn,
								cl_hton16
								(slid));
					dest_port =
					    osm_get_port_by_lid(p_mgr->p_subn,
								cl_hton16
								(dlid));

					/* remove path from current cdg / vl */
					err =
					    remove_path_from_cdg(&
								 (cdg[test_vl]),
								 src_port, slid,
								 dest_port,
								 dlid);
					if (err) {
						OSM_LOG(p_mgr->p_log,
							OSM_LOG_ERROR,
							"ERR AD44: something went wrong in remove_path_from_cdg(...)\n");
						goto ERROR;
					}

					/* add path to next cdg / vl */
					err =
					    update_channel_dep_graph(&
								     (cdg
								      [test_vl +
								       1]),
								     src_port,
								     slid,
								     dest_port,
								     dlid);
					if (err) {
						OSM_LOG(p_mgr->p_log,
							OSM_LOG_ERROR,
							"ERR AD14: cannot allocate memory for cdg node or link in update_channel_dep_graph(...)\n");
						goto ERROR;
					}
					vltable_insert(srcdest2vl_table,
						       cl_hton16(slid),
						       cl_hton16(dlid),
						       test_vl + 1);
				}

				if (weakest_link->num_pairs)
					free(weakest_link->srcdest_pairs);
				if (weakest_link)
					free(weakest_link);
			}

			start_here = cycle;
		}
	}

	/* test the last avail cdg for a cycle;
	   if there is one, than vl_needed > vl_avail
	 */
	start_here = cdg[vl_avail - 1];
	if (start_here) {
		cycle =
		    search_cycle_in_channel_dep_graph(cdg[vl_avail - 1],
						      start_here);
		if (cycle) {
			vl_needed = vl_avail + 1;
		}
	}

	OSM_LOG(p_mgr->p_log, OSM_LOG_INFO,
		"Virtual Lanes needed: %" PRIu8 "\n", vl_needed);
	if (OSM_LOG_IS_ACTIVE_V2(p_mgr->p_log, OSM_LOG_INFO)) {
		OSM_LOG(p_mgr->p_log, OSM_LOG_INFO,
			"Paths per VL (before balancing):\n");
		for (i = 0; i < vl_avail; i++)
			OSM_LOG(p_mgr->p_log, OSM_LOG_INFO,
				"   %" PRIu32 ". lane: %" PRIu64 "\n", i,
				paths_per_vl[i]);
	}

	OSM_LOG(p_mgr->p_log, OSM_LOG_VERBOSE,
		"Balancing the paths on the available Virtual Lanes\n");

	/* optimal balancing virtual lanes, under condition: no additional cycle checks;
	   sl/vl != 0 might be assigned to loopback packets (i.e. slid/dlid on the
	   same port for lmc>0), but thats no problem, see IBAS 10.2.2.3
	 */
	split_count = (uint8_t *) calloc(vl_avail, sizeof(uint8_t));
	if (!split_count) {
		OSM_LOG(p_mgr->p_log, OSM_LOG_ERROR,
			"ERR AD24: cannot allocate memory for split_count, skip balancing\n");
		err = 1;
		goto ERROR;
	}
	/* initial state: paths for VLs won't be separated */
	for (i = 0; i < ((vl_needed < vl_avail) ? vl_needed : vl_avail); i++)
		split_count[i] = 1;
	dfsssp_ctx->vl_split_count = split_count;
	/* balancing is necessary if we have empty VLs */
	if (vl_needed < vl_avail) {
		/* split paths of VLs until we find an equal distribution */
		for (i = vl_needed; i < vl_avail; i++) {
			/* find VL with most paths in it */
			vl = 0;
			most_avg_paths = 0.0;
			for (test_vl = 0; test_vl < vl_needed; test_vl++) {
				if (most_avg_paths <
				    ((double)paths_per_vl[test_vl] /
				    split_count[test_vl])) {
					vl = test_vl;
					most_avg_paths =
						(double)paths_per_vl[test_vl] /
						split_count[test_vl];
				}
			}
			split_count[vl]++;
		}
		/* change the VL assignment depending on split_count for
		   all VLs except VL 0
		 */
		for (from = vl_needed - 1; from > 0; from--) {
			/* how much space needed for others? */
			to = 0;
			for (i = 0; i < from; i++)
				to += split_count[i];
			count = paths_per_vl[from];
			vltable_change_vl(srcdest2vl_table, from, to, count);
			/* change also the information within the split_count
			   array; this is important for fast calculation later
			 */
			split_count[to] = split_count[from];
			split_count[from] = 0;
			paths_per_vl[to] = paths_per_vl[from];
			paths_per_vl[from] = 0;
		}
	} else if (vl_needed > vl_avail) {
		/* routing not possible, a further development would be the LASH-TOR approach (update: LASH-TOR isn't possible, there is a mistake in the theory) */
		OSM_LOG(p_mgr->p_log, OSM_LOG_ERROR,
			"ERR AD25: Not enough VLs available (avail=%d, needed=%d); Stopping dfsssp routing!\n",
			vl_avail, vl_needed);
		err = 1;
		goto ERROR;
	}
	/* else { no balancing } */

	if (OSM_LOG_IS_ACTIVE_V2(p_mgr->p_log, OSM_LOG_DEBUG)) {
		OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
			"Virtual Lanes per src/dest combination after balancing:\n");
		vltable_print(p_mgr, srcdest2vl_table);
	}
	if (OSM_LOG_IS_ACTIVE_V2(p_mgr->p_log, OSM_LOG_INFO)) {
		OSM_LOG(p_mgr->p_log, OSM_LOG_INFO,
			"Approx. #paths per VL (after balancing):\n");
		j = 0;
		count = 1; /* to prevent div. by 0 */
		for (i = 0; i < vl_avail; i++) {
			if (split_count[i] > 0) {
				j = i;
				count = split_count[i];
			}
			OSM_LOG(p_mgr->p_log, OSM_LOG_INFO,
				"   %" PRIu32 ". lane: %" PRIu64 "\n", i,
				paths_per_vl[j] / count);
		}
	}

	free(paths_per_vl);

	/* deallocate channel dependency graphs */
	for (i = 0; i < vl_avail; i++)
		cdg_dealloc(&cdg[i]);
	free(cdg);

	OSM_LOG_EXIT(p_mgr->p_log);
	return 0;

ERROR:
	free(paths_per_vl);

	for (i = 0; i < vl_avail; i++)
		cdg_dealloc(&cdg[i]);
	free(cdg);

	vltable_dealloc(&srcdest2vl_table);
	dfsssp_ctx->srcdest2vl_table = NULL;

	return err;
}