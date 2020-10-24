static void
cache_restore_ucast_info(osm_ucast_mgr_t * p_mgr,
			 cache_switch_t * p_cache_sw, osm_switch_t * p_sw)
{
	if (!p_mgr->cache_valid)
		return;

	/* when seting unicast info, the cached port
	   should have all the required info */
	CL_ASSERT(p_cache_sw->max_lid_ho && p_cache_sw->lft &&
		  p_cache_sw->num_hops && p_cache_sw->hops);

	p_sw->max_lid_ho = p_cache_sw->max_lid_ho;

	if (p_sw->new_lft)
		free(p_sw->new_lft);
	p_sw->new_lft = p_cache_sw->lft;
	p_cache_sw->lft = NULL;

	p_sw->num_hops = p_cache_sw->num_hops;
	p_cache_sw->num_hops = 0;
	if (p_sw->hops)
		free(p_sw->hops);
	p_sw->hops = p_cache_sw->hops;
	p_cache_sw->hops = NULL;

	p_sw->need_update = 2;
}