static void lid_mgr_validate_db(IN osm_lid_mgr_t * p_mgr)
{
	cl_qlist_t guids;
	osm_db_guid_elem_t *p_item;
	uint16_t lid;
	uint16_t min_lid;
	uint16_t max_lid;
	uint16_t lmc_mask;
	boolean_t lids_ok;

	OSM_LOG_ENTER(p_mgr->p_log);

	lmc_mask = ~((1 << p_mgr->p_subn->opt.lmc) - 1);

	cl_qlist_init(&guids);

	if (osm_db_guid2lid_guids(p_mgr->p_g2l, &guids)) {
		OSM_LOG(p_mgr->p_log, OSM_LOG_ERROR, "ERR 0310: "
			"could not get guid list\n");
		goto Exit;
	}

	while ((p_item = (osm_db_guid_elem_t *) cl_qlist_remove_head(&guids))
	       != (osm_db_guid_elem_t *) cl_qlist_end(&guids)) {
		if (osm_db_guid2lid_get(p_mgr->p_g2l, p_item->guid,
					&min_lid, &max_lid))
			OSM_LOG(p_mgr->p_log, OSM_LOG_ERROR, "ERR 0311: "
				"could not get lid for guid:0x%016" PRIx64 "\n",
				p_item->guid);
		else {
			lids_ok = TRUE;

			if (min_lid > max_lid || min_lid == 0
			    || p_item->guid == 0
			    || max_lid > p_mgr->p_subn->max_ucast_lid_ho) {
				OSM_LOG(p_mgr->p_log, OSM_LOG_ERROR,
					"ERR 0312: "
					"Illegal LID range [%u:%u] for "
					"guid:0x%016" PRIx64 "\n", min_lid,
					max_lid, p_item->guid);
				lids_ok = FALSE;
			} else if (min_lid != max_lid
				   && (min_lid & lmc_mask) != min_lid) {
				/* check that if the lids define a range that is
				   valid for the current LMC mask */
				OSM_LOG(p_mgr->p_log, OSM_LOG_ERROR,
					"ERR 0313: "
					"LID range [%u:%u] for guid:0x%016"
					PRIx64
					" is not aligned according to mask:0x%04x\n",
					min_lid, max_lid, p_item->guid,
					lmc_mask);
				lids_ok = FALSE;
			} else {
				/* check if the lids were not previously assigned */
				for (lid = min_lid; lid <= max_lid; lid++) {
					if (p_mgr->used_lids[lid]) {
						OSM_LOG(p_mgr->p_log,
							OSM_LOG_ERROR,
							"ERR 0314: "
							"0x%04x for guid:0x%016"
							PRIx64
							" was previously used\n",
							lid, p_item->guid);
						lids_ok = FALSE;
					}
				}
			}

			if (lids_ok)
				/* mark that it was visited */
				for (lid = min_lid; lid <= max_lid; lid++)
					p_mgr->used_lids[lid] = 1;
			else if (osm_db_guid2lid_delete(p_mgr->p_g2l,
							p_item->guid))
				OSM_LOG(p_mgr->p_log, OSM_LOG_ERROR,
					"ERR 0315: failed to delete entry for "
					"guid:0x%016" PRIx64 "\n",
					p_item->guid);
		}		/* got a lid */
		free(p_item);
	}			/* all guids */
Exit:
	OSM_LOG_EXIT(p_mgr->p_log);
}