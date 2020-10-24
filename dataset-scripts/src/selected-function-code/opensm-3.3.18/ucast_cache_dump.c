static void ucast_cache_dump(osm_ucast_mgr_t * p_mgr)
{
	cache_switch_t *p_sw;
	unsigned i;

	OSM_LOG_ENTER(p_mgr->p_log);

	if (!OSM_LOG_IS_ACTIVE_V2(p_mgr->p_log, OSM_LOG_DEBUG))
		goto Exit;

	OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
		"Dumping missing nodes/links as logged by unicast cache:\n");
	for (p_sw = (cache_switch_t *) cl_qmap_head(&p_mgr->cache_sw_tbl);
	     p_sw != (cache_switch_t *) cl_qmap_end(&p_mgr->cache_sw_tbl);
	     p_sw = (cache_switch_t *) cl_qmap_next(&p_sw->map_item)) {

		OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
			"\t Switch lid %u %s%s\n",
			cache_sw_get_base_lid_ho(p_sw),
			(cache_sw_is_leaf(p_sw)) ? "[leaf switch] " : "",
			(p_sw->dropped) ? "[whole switch missing]" : "");

		for (i = 1; i < p_sw->num_ports; i++)
			if (p_sw->ports[i].remote_lid_ho > 0)
				OSM_LOG(p_mgr->p_log,
					OSM_LOG_DEBUG,
					"\t     - port %u -> lid %u %s\n",
					i, p_sw->ports[i].remote_lid_ho,
					(p_sw->ports[i].is_leaf) ?
					"[remote node is leaf]" : "");
	}
Exit:
	OSM_LOG_EXIT(p_mgr->p_log);
}