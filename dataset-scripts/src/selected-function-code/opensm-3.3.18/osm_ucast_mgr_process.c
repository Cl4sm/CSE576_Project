int osm_ucast_mgr_process(IN osm_ucast_mgr_t * p_mgr)
{
	osm_opensm_t *p_osm;
	struct osm_routing_engine *p_routing_eng;
	cl_qmap_t *p_sw_guid_tbl;
	int failed = 0;

	OSM_LOG_ENTER(p_mgr->p_log);

	p_sw_guid_tbl = &p_mgr->p_subn->sw_guid_tbl;
	p_osm = p_mgr->p_subn->p_osm;
	p_routing_eng = p_osm->routing_engine_list;

	CL_PLOCK_EXCL_ACQUIRE(p_mgr->p_lock);

	/*
	   If there are no switches in the subnet, we are done.
	 */
	if (cl_qmap_count(p_sw_guid_tbl) == 0 ||
	    ucast_mgr_setup_all_switches(p_mgr->p_subn) < 0)
		goto Exit;

	failed = -1;
	p_osm->routing_engine_used = NULL;
	while (p_routing_eng) {
		failed = ucast_mgr_route(p_routing_eng, p_osm);
		if (!failed)
			break;
		p_routing_eng = p_routing_eng->next;
	}

	if (!p_osm->routing_engine_used &&
	    p_osm->no_fallback_routing_engine != TRUE) {
		/* If configured routing algorithm failed, use default MinHop */
		failed = ucast_mgr_route(p_osm->default_routing_engine, p_osm);
	}

	if (p_osm->routing_engine_used) {
		OSM_LOG(p_mgr->p_log, OSM_LOG_INFO,
			"%s tables configured on all switches\n",
			osm_routing_engine_type_str(p_osm->
						    routing_engine_used->type));

		if (p_mgr->p_subn->opt.use_ucast_cache)
			p_mgr->cache_valid = TRUE;
	} else {
		p_mgr->p_subn->subnet_initialization_error = TRUE;
		OSM_LOG(p_mgr->p_log, OSM_LOG_ERROR,
			"No routing engine able to successfully configure "
			" switch tables on current fabric\n");
	}
Exit:
	CL_PLOCK_RELEASE(p_mgr->p_lock);
	OSM_LOG_EXIT(p_mgr->p_log);
	return failed;
}