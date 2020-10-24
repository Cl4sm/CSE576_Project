ib_api_status_t osm_opensm_init_finish(IN osm_opensm_t * p_osm,
				       IN const osm_subn_opt_t * p_opt)
{
	ib_api_status_t status;

	osm_opensm_construct_finish(p_osm);

	p_osm->subn.sm_port_guid = p_opt->guid;

	status = osm_mad_pool_init(&p_osm->mad_pool);
	if (status != IB_SUCCESS)
		goto Exit;

	status = osm_vl15_init(&p_osm->vl15, p_osm->p_vendor,
			       &p_osm->log, &p_osm->stats, &p_osm->subn,
			       p_opt->max_wire_smps, p_opt->max_wire_smps2,
			       p_opt->max_smps_timeout);
	if (status != IB_SUCCESS)
		goto Exit;

	status = osm_sm_init(&p_osm->sm, &p_osm->subn, &p_osm->db,
			     p_osm->p_vendor, &p_osm->mad_pool, &p_osm->vl15,
			     &p_osm->log, &p_osm->stats, &p_osm->disp,
			     &p_osm->lock);
	if (status != IB_SUCCESS)
		goto Exit;

	status = osm_sa_init(&p_osm->sm, &p_osm->sa, &p_osm->subn,
			     p_osm->p_vendor, &p_osm->mad_pool, &p_osm->log,
			     &p_osm->stats, &p_osm->disp, &p_osm->lock);
	if (status != IB_SUCCESS)
		goto Exit;

	cl_qlist_init(&p_osm->plugin_list);

	if (p_opt->event_plugin_name)
		load_plugins(p_osm, p_opt->event_plugin_name);

#ifdef ENABLE_OSM_PERF_MGR
	status = osm_perfmgr_init(&p_osm->perfmgr, p_osm, p_opt);
	if (status != IB_SUCCESS)
		goto Exit;
#endif				/* ENABLE_OSM_PERF_MGR */

	status = osm_congestion_control_init(&p_osm->cc,
					     p_osm, p_opt);
	if (status != IB_SUCCESS)
		goto Exit;

	p_osm->no_fallback_routing_engine = FALSE;

	setup_routing_engines(p_osm, p_opt->routing_engine_names);

	p_osm->routing_engine_used = OSM_ROUTING_ENGINE_TYPE_NONE;

	p_osm->node_name_map = open_node_name_map(p_opt->node_name_map_name);

Exit:
	OSM_LOG(&p_osm->log, OSM_LOG_FUNCS, "]\n");	/* Format Waived */
	return status;
}