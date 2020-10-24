static void fabric_make_indexing(IN ftree_fabric_t * p_ftree)
{
	ftree_sw_t *p_remote_sw;
	ftree_sw_t *p_sw = NULL;
	ftree_sw_t *p_next_sw;
	ftree_tuple_t new_tuple;
	uint32_t i;
	cl_list_t bfs_list;

	OSM_LOG_ENTER(&p_ftree->p_osm->log);

	OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_VERBOSE,
		"Starting FatTree indexing\n");

	/* using the first leaf switch as a starting point for indexing algorithm. */
	p_next_sw = (ftree_sw_t *) cl_qmap_head(&p_ftree->sw_tbl);
	while (p_next_sw != (ftree_sw_t *) cl_qmap_end(&p_ftree->sw_tbl)) {
		p_sw = p_next_sw;
		if (p_sw->is_leaf)
			break;
		p_next_sw = (ftree_sw_t *) cl_qmap_next(&p_sw->map_item);
	}

	CL_ASSERT(p_next_sw != (ftree_sw_t *) cl_qmap_end(&p_ftree->sw_tbl));

	/* Assign the first tuple to the switch that is used as BFS starting point.
	   The tuple will be as follows: [rank].0.0.0...
	   This fuction also adds the switch it into the switch_by_tuple table. */
	fabric_assign_first_tuple(p_ftree, p_sw);

	OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_VERBOSE,
		"Indexing starting point:\n"
		"                                            - Switch rank  : %u\n"
		"                                            - Switch index : %s\n"
		"                                            - Node LID     : %u\n"
		"                                            - Node GUID    : 0x%016"
		PRIx64 "\n", p_sw->rank, tuple_to_str(p_sw->tuple),
		p_sw->base_lid, sw_get_guid_ho(p_sw));

	/*
	 * Now run BFS and assign indexes to all switches
	 * Pseudo code of the algorithm is as follows:
	 *
	 *  * Add first switch to BFS queue
	 *  * While (BFS queue not empty)
	 *      - Pop the switch from the head of the queue
	 *      - Scan all the downward and upward ports
	 *      - For each port
	 *          + Get the remote switch
	 *          + Assign index to the remote switch
	 *          + Add remote switch to the BFS queue
	 */

	cl_list_init(&bfs_list, cl_qmap_count(&p_ftree->sw_tbl));
	cl_list_insert_tail(&bfs_list, p_sw);

	while (!cl_is_list_empty(&bfs_list)) {
		p_sw = (ftree_sw_t *) cl_list_remove_head(&bfs_list);

		/* Discover all the nodes from ports that are pointing down */

		if (p_sw->rank >= p_ftree->leaf_switch_rank) {
			/* whether downward ports are pointing to CAs or switches,
			   we don't assign indexes to switches that are located
			   lower than leaf switches */
		} else {
			/* This is not the leaf switch */
			for (i = 0; i < p_sw->down_port_groups_num; i++) {
				/* Work with port groups that are pointing to switches only.
				   No need to assign indexing to HCAs */
				if (p_sw->
				    down_port_groups[i]->remote_node_type !=
				    IB_NODE_TYPE_SWITCH)
					continue;

				p_remote_sw =
				    p_sw->down_port_groups[i]->
				    remote_hca_or_sw.p_sw;
				if (tuple_assigned(p_remote_sw->tuple)) {
					/* this switch has been already indexed */
					continue;
				}
				/* allocate new tuple */
				fabric_get_new_tuple(p_ftree, new_tuple,
						     p_sw->tuple,
						     FTREE_DIRECTION_DOWN);
				/* Assign the new tuple to the remote switch.
				   This fuction also adds the switch into the switch_by_tuple table. */
				fabric_assign_tuple(p_ftree, p_remote_sw,
						    new_tuple);

				/* add the newly discovered switch to the BFS queue */
				cl_list_insert_tail(&bfs_list, p_remote_sw);
			}
			/* Done assigning indexes to all the remote switches
			   that are pointed by the downgoing ports.
			   Now sort port groups according to remote index. */
			qsort(p_sw->down_port_groups,	/* array */
			      p_sw->down_port_groups_num,	/* number of elements */
			      sizeof(ftree_port_group_t *),	/* size of each element */
			      compare_port_groups_by_remote_switch_index);	/* comparator */
		}

		/* Done indexing switches from ports that go down.
		   Now do the same with ports that are pointing up. */

		if (p_sw->rank != 0) {
			/* This is not the root switch, which means that all the ports
			   that are pointing up are taking us to another switches. */
			for (i = 0; i < p_sw->up_port_groups_num; i++) {
				p_remote_sw =
				    p_sw->up_port_groups[i]->
				    remote_hca_or_sw.p_sw;
				if (tuple_assigned(p_remote_sw->tuple))
					continue;
				/* allocate new tuple */
				fabric_get_new_tuple(p_ftree, new_tuple,
						     p_sw->tuple,
						     FTREE_DIRECTION_UP);
				/* Assign the new tuple to the remote switch.
				   This fuction also adds the switch to the
				   switch_by_tuple table. */
				fabric_assign_tuple(p_ftree,
						    p_remote_sw, new_tuple);
				/* add the newly discovered switch to the BFS queue */
				cl_list_insert_tail(&bfs_list, p_remote_sw);
			}
			/* Done assigning indexes to all the remote switches
			   that are pointed by the upgoing ports.
			   Now sort port groups according to remote index. */
			qsort(p_sw->up_port_groups,	/* array */
			      p_sw->up_port_groups_num,	/* number of elements */
			      sizeof(ftree_port_group_t *),	/* size of each element */
			      compare_port_groups_by_remote_switch_index);	/* comparator */
		}
		/* Done assigning indexes to all the switches that are directly connected
		   to the current switch - go to the next switch in the BFS queue */
	}
	cl_list_destroy(&bfs_list);

	OSM_LOG_EXIT(&p_ftree->p_osm->log);
}				/* fabric_make_indexing() */