static int lid_mgr_init_sweep(IN osm_lid_mgr_t * p_mgr)
{
	cl_ptr_vector_t *p_discovered_vec = &p_mgr->p_subn->port_lid_tbl;
	uint16_t max_defined_lid, max_persistent_lid, max_discovered_lid;
	uint16_t disc_min_lid, disc_max_lid, db_min_lid, db_max_lid;
	int status = 0;
	cl_list_item_t *p_item;
	boolean_t is_free;
	osm_lid_mgr_range_t *p_range = NULL;
	osm_port_t *p_port;
	cl_qmap_t *p_port_guid_tbl;
	uint8_t lmc_num_lids = (uint8_t) (1 << p_mgr->p_subn->opt.lmc);
	uint16_t lmc_mask, req_lid, num_lids, lid;

	OSM_LOG_ENTER(p_mgr->p_log);

	lmc_mask = ~((1 << p_mgr->p_subn->opt.lmc) - 1);

	/* We must discard previous guid2lid db if this is the first master
	 * sweep and reassign_lids option is TRUE.
	 * If we came out of standby and honor_guid2lid_file option is TRUE, we
	 * must restore guid2lid db. Otherwise if honor_guid2lid_file option is
	 * FALSE we must discard previous guid2lid db.
	 */
	if (p_mgr->p_subn->first_time_master_sweep == TRUE &&
	    p_mgr->p_subn->opt.reassign_lids == TRUE) {
		osm_db_clear(p_mgr->p_g2l);
		memset(p_mgr->used_lids, 0, sizeof(p_mgr->used_lids));
	} else if (p_mgr->p_subn->coming_out_of_standby == TRUE) {
		osm_db_clear(p_mgr->p_g2l);
		memset(p_mgr->used_lids, 0, sizeof(p_mgr->used_lids));
		if (p_mgr->p_subn->opt.honor_guid2lid_file == FALSE)
			OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
				"Ignore guid2lid file when coming out of standby\n");
		else {
			OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
				"Honor current guid2lid file when coming out "
				"of standby\n");
			if (osm_db_restore(p_mgr->p_g2l))
				OSM_LOG(p_mgr->p_log, OSM_LOG_ERROR,
					"ERR 0306: "
					"Error restoring Guid-to-Lid "
					"persistent database. Ignoring it\n");
			lid_mgr_validate_db(p_mgr);
		}
	}

	/* we need to cleanup the empty ranges list */
	while ((p_item = cl_qlist_remove_head(&p_mgr->free_ranges)) !=
	       cl_qlist_end(&p_mgr->free_ranges))
		free((osm_lid_mgr_range_t *) p_item);

	/* first clean up the port_by_lid_tbl */
	for (lid = 0; lid < cl_ptr_vector_get_size(p_discovered_vec); lid++)
		cl_ptr_vector_set(p_discovered_vec, lid, NULL);

	/* we if are in the first sweep and in reassign lids mode
	   we should ignore all the available info and simply define one
	   huge empty range */
	if (p_mgr->p_subn->first_time_master_sweep == TRUE &&
	    p_mgr->p_subn->opt.reassign_lids == TRUE) {
		OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
			"Skipping all lids as we are reassigning them\n");
		p_range = malloc(sizeof(osm_lid_mgr_range_t));
		if (p_range)
			p_range->min_lid = 1;
		goto AfterScanningLids;
	}

	/* go over all discovered ports and mark their entries */
	p_port_guid_tbl = &p_mgr->p_subn->port_guid_tbl;

	for (p_port = (osm_port_t *) cl_qmap_head(p_port_guid_tbl);
	     p_port != (osm_port_t *) cl_qmap_end(p_port_guid_tbl);
	     p_port = (osm_port_t *) cl_qmap_next(&p_port->map_item)) {
		osm_port_get_lid_range_ho(p_port, &disc_min_lid, &disc_max_lid);
		disc_min_lid = trim_lid(disc_min_lid);
		disc_max_lid = trim_lid(disc_max_lid);
		for (lid = disc_min_lid; lid <= disc_max_lid; lid++)
			cl_ptr_vector_set(p_discovered_vec, lid, p_port);
		/* make sure the guid2lid entry is valid. If not, clean it. */
		if (osm_db_guid2lid_get(p_mgr->p_g2l,
					cl_ntoh64(osm_port_get_guid(p_port)),
					&db_min_lid, &db_max_lid))
			continue;

		if (!p_port->p_node->sw ||
		    osm_switch_sp0_is_lmc_capable(p_port->p_node->sw,
						  p_mgr->p_subn))
			num_lids = lmc_num_lids;
		else
			num_lids = 1;

		if (num_lids != 1 &&
		    ((db_min_lid & lmc_mask) != db_min_lid ||
		     db_max_lid - db_min_lid + 1 < num_lids)) {
			/* Not aligned, or not wide enough, then remove the entry */
			OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
				"Cleaning persistent entry for guid:"
				"0x%016" PRIx64 " illegal range:[0x%x:0x%x]\n",
				cl_ntoh64(osm_port_get_guid(p_port)),
				db_min_lid, db_max_lid);
			osm_db_guid2lid_delete(p_mgr->p_g2l,
					       cl_ntoh64
					       (osm_port_get_guid(p_port)));
			for (lid = db_min_lid; lid <= db_max_lid; lid++)
				p_mgr->used_lids[lid] = 0;
		}
	}

	/*
	   Our task is to find free lid ranges.
	   A lid can be used if
	   1. a persistent assignment exists
	   2. the lid is used by a discovered port that does not have a
	   persistent assignment.

	   scan through all lid values of both the persistent table and
	   discovered table.
	   If the lid has an assigned port in the discovered table:
	   * make sure the lid matches the persistent table, or
	   * there is no other persistent assignment for that lid.
	   * else cleanup the port_by_lid_tbl, mark this as empty range.
	   Else if the lid does not have an entry in the persistent table
	   mark it as free.
	 */

	/* find the range of lids to scan */
	max_discovered_lid =
	    (uint16_t) cl_ptr_vector_get_size(p_discovered_vec);
	max_persistent_lid = sizeof(p_mgr->used_lids) - 1;

	/* but the vectors have one extra entry for lid=0 */
	if (max_discovered_lid)
		max_discovered_lid--;

	if (max_persistent_lid > max_discovered_lid)
		max_defined_lid = max_persistent_lid;
	else
		max_defined_lid = max_discovered_lid;

	for (lid = 1; lid <= max_defined_lid; lid++) {
		is_free = TRUE;
		/* first check to see if the lid is used by a persistent assignment */
		if (lid <= max_persistent_lid && p_mgr->used_lids[lid]) {
			OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
				"0x%04x is not free as its mapped by the "
				"persistent db\n", lid);
			is_free = FALSE;
			/* check this is a discovered port */
		} else if (lid <= max_discovered_lid &&
			   (p_port = cl_ptr_vector_get(p_discovered_vec,
						       lid))) {
			/* we have a port. Now lets see if we can preserve its lid range. */
			/* For that, we need to make sure:
			   1. The port has a (legal) persistency entry. Then the
			   local lid is free (we will use the persistency value).
			   2. Can the port keep its local assignment?
			   a. Make sure the lid is aligned.
			   b. Make sure all needed lids (for the lmc) are free
			   according to persistency table.
			 */
			/* qualify the guid of the port is not persistently
			   mapped to another range */
			if (!osm_db_guid2lid_get(p_mgr->p_g2l,
						 cl_ntoh64
						 (osm_port_get_guid(p_port)),
						 &db_min_lid, &db_max_lid)) {
				OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
					"0x%04x is free as it was "
					"discovered but mapped by the "
					"persistent db to [0x%04x:0x%04x]\n",
					lid, db_min_lid, db_max_lid);
			} else {
				/* can the port keep its assignment ? */
				/* get the lid range of that port, and the
				   required number of lids we are about to
				   assign to it */
				osm_port_get_lid_range_ho(p_port,
							  &disc_min_lid,
							  &disc_max_lid);
				if (!p_port->p_node->sw ||
				    osm_switch_sp0_is_lmc_capable
				    (p_port->p_node->sw, p_mgr->p_subn)) {
					disc_max_lid =
					    disc_min_lid + lmc_num_lids - 1;
					num_lids = lmc_num_lids;
				} else
					num_lids = 1;

				/* Make sure the lid is aligned */
				if (num_lids != 1
				    && (disc_min_lid & lmc_mask) !=
				    disc_min_lid) {
					/* The lid cannot be used */
					OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
						"0x%04x is free as it was "
						"discovered but not aligned\n",
						lid);
				} else {
					/* check that all needed lids are not persistently mapped */
					is_free = FALSE;
					for (req_lid = disc_min_lid + 1;
					     req_lid <= disc_max_lid;
					     req_lid++) {
						if (req_lid <=
						    max_persistent_lid &&
						    p_mgr->used_lids[req_lid]) {
							OSM_LOG(p_mgr->p_log,
								OSM_LOG_DEBUG,
								"0x%04x is free as it was discovered "
								"but mapped\n",
								lid);
							is_free = TRUE;
							break;
						}
					}

					if (is_free == FALSE) {
						/* This port will use its local lid, and consume the entire required lid range.
						   Thus we can skip that range. */
						/* If the disc_max_lid is greater then lid, we can skip right to it,
						   since we've done all neccessary checks on the lids in between. */
						if (disc_max_lid > lid)
							lid = disc_max_lid;
					}
				}
			}
		}

		if (is_free) {
			if (p_range)
				p_range->max_lid = lid;
			else {
				p_range = malloc(sizeof(osm_lid_mgr_range_t));
				if (p_range) {
					p_range->min_lid = lid;
					p_range->max_lid = lid;
				}
			}
		/* this lid is used so we need to finalize the previous free range */
		} else if (p_range) {
			cl_qlist_insert_tail(&p_mgr->free_ranges,
					     &p_range->item);
			OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
				"new free lid range [%u:%u]\n",
				p_range->min_lid, p_range->max_lid);
			p_range = NULL;
		}
	}

AfterScanningLids:
	/* after scanning all known lids we need to extend the last range
	   to the max allowed lid */
	if (!p_range) {
		p_range = malloc(sizeof(osm_lid_mgr_range_t));
		/*
		   The p_range can be NULL in one of 2 cases:
		   1. If max_defined_lid == 0. In this case, we want the
		   entire range.
		   2. If all lids discovered in the loop where mapped. In this
		   case, no free range exists and we want to define it after the
		   last mapped lid.
		 */
		if (p_range)
			p_range->min_lid = lid;
	}
	if (p_range) {
		p_range->max_lid = p_mgr->p_subn->max_ucast_lid_ho;
		cl_qlist_insert_tail(&p_mgr->free_ranges, &p_range->item);
		OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
			"final free lid range [%u:%u]\n",
			p_range->min_lid, p_range->max_lid);
	}

	OSM_LOG_EXIT(p_mgr->p_log);
	return status;
}