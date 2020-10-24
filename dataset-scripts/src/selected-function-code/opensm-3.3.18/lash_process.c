static int lash_process(void *context)
{
	lash_t *p_lash = context;
	osm_log_t *p_log = &p_lash->p_osm->log;
	int status = 0;

	OSM_LOG_ENTER(p_log);

	p_lash->balance_limit = 6;

	/* everything starts here */
	lash_cleanup(p_lash);

	status = discover_network_properties(p_lash);
	if (status)
		goto Exit;

	status = init_lash_structures(p_lash);
	if (status)
		goto Exit;

	process_switches(p_lash);

	status = lash_core(p_lash);
	if (status)
		goto Exit;

	populate_fwd_tbls(p_lash);

Exit:
	if (p_lash->vl_min)
		free_lash_structures(p_lash);
	OSM_LOG_EXIT(p_log);

	return status;
}