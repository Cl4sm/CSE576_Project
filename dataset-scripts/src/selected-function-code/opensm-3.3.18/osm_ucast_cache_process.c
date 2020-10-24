int osm_ucast_cache_process(osm_ucast_mgr_t * p_mgr)
{
	cl_qmap_t *tbl = &p_mgr->p_subn->sw_guid_tbl;
	cl_map_item_t *item;
	osm_switch_t *p_sw;
	uint16_t lft_size;

	if (!p_mgr->p_subn->opt.use_ucast_cache)
		return 1;

	ucast_cache_validate(p_mgr);
	if (!p_mgr->cache_valid)
		return 1;

	OSM_LOG(p_mgr->p_log, OSM_LOG_INFO,
		"Configuring switch tables using cached routing\n");

	for (item = cl_qmap_head(tbl); item != cl_qmap_end(tbl);
	     item = cl_qmap_next(item)) {
		p_sw = (osm_switch_t *) item;

		if (p_sw->need_update) {
			if (!p_sw->new_lft)
				/* no new routing was recently calculated for this
				   switch, but the LFT needs to be updated anyway */
				p_sw->new_lft = p_sw->lft;

			lft_size = (p_sw->max_lid_ho / IB_SMP_DATA_SIZE + 1)
				   * IB_SMP_DATA_SIZE;
			p_sw->lft = malloc(lft_size);
			if (!p_sw->lft)
				return IB_INSUFFICIENT_MEMORY;
			p_sw->lft_size = lft_size;
			memset(p_sw->lft, OSM_NO_PATH, p_sw->lft_size);
		}

	}

	osm_ucast_mgr_set_fwd_tables(p_mgr);

	return 0;
}