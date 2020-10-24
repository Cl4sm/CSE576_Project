void osm_ucast_cache_invalidate(osm_ucast_mgr_t * p_mgr)
{
	cache_switch_t *p_sw;
	cache_switch_t *p_next_sw;

	OSM_LOG_ENTER(p_mgr->p_log);

	if (!p_mgr->cache_valid)
		goto Exit;

	p_mgr->cache_valid = FALSE;

	p_next_sw = (cache_switch_t *) cl_qmap_head(&p_mgr->cache_sw_tbl);
	while (p_next_sw !=
	       (cache_switch_t *) cl_qmap_end(&p_mgr->cache_sw_tbl)) {
		p_sw = p_next_sw;
		p_next_sw = (cache_switch_t *) cl_qmap_next(&p_sw->map_item);
		cache_sw_destroy(p_sw);
	}
	cl_qmap_remove_all(&p_mgr->cache_sw_tbl);

	OSM_LOG(p_mgr->p_log, OSM_LOG_VERBOSE, "Unicast Cache invalidated\n");
Exit:
	OSM_LOG_EXIT(p_mgr->p_log);
}