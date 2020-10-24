static void lid_mgr_find_free_lid_range(IN osm_lid_mgr_t * p_mgr,
					IN uint8_t num_lids,
					OUT uint16_t * p_min_lid,
					OUT uint16_t * p_max_lid)
{
	uint16_t lid;
	cl_list_item_t *p_item;
	cl_list_item_t *p_next_item;
	osm_lid_mgr_range_t *p_range = NULL;
	uint8_t lmc_num_lids;
	uint16_t lmc_mask;

	OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG, "LMC = %u, number LIDs = %u\n",
		p_mgr->p_subn->opt.lmc, num_lids);

	lmc_num_lids = (1 << p_mgr->p_subn->opt.lmc);
	lmc_mask = ~((1 << p_mgr->p_subn->opt.lmc) - 1);

	/*
	   Search the list of free lid ranges for a range which is big enough
	 */
	p_item = cl_qlist_head(&p_mgr->free_ranges);
	while (p_item != cl_qlist_end(&p_mgr->free_ranges)) {
		p_next_item = cl_qlist_next(p_item);
		p_range = (osm_lid_mgr_range_t *) p_item;

		lid = p_range->min_lid;

		/* if we require more then one lid we must align to LMC */
		if (num_lids > 1) {
			if ((lid & lmc_mask) != lid)
				lid = (lid + lmc_num_lids) & lmc_mask;
		}

		/* but we can be out of the range */
		if (lid + num_lids - 1 <= p_range->max_lid) {
			/* ok let us use that range */
			if (lid + num_lids - 1 == p_range->max_lid) {
				/* we consumed the entire range */
				cl_qlist_remove_item(&p_mgr->free_ranges,
						     p_item);
				free(p_item);
			} else
				/* only update the available range */
				p_range->min_lid = lid + num_lids;

			*p_min_lid = lid;
			*p_max_lid = (uint16_t) (lid + num_lids - 1);
			return;
		}
		p_item = p_next_item;
	}

	/*
	   Couldn't find a free range of lids.
	 */
	*p_min_lid = *p_max_lid = 0;
	/* if we run out of lids, give an error and abort! */
	OSM_LOG(p_mgr->p_log, OSM_LOG_ERROR, "ERR 0307: "
		"OPENSM RAN OUT OF LIDS!!!\n");
	CL_ASSERT(0);
}