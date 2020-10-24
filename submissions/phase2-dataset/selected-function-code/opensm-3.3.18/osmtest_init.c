ib_api_status_t
osmtest_init(IN osmtest_t * const p_osmt,
	     IN const osmtest_opt_t * const p_opt,
	     IN const osm_log_level_t log_flags)
{
	ib_api_status_t status;

	/* Can't use log macros here, since we're initializing the log. */
	osmtest_construct(p_osmt);

	status = osm_log_init_v2(&p_osmt->log, p_opt->force_log_flush,
				 0x0001, p_opt->log_file, 0, TRUE);
	if (status != IB_SUCCESS)
		return (status);

	/* but we do not want any extra stuff here */
	osm_log_set_level(&p_osmt->log, log_flags);

	OSM_LOG(&p_osmt->log, OSM_LOG_FUNCS, "[\n");

	p_osmt->opt = *p_opt;

	status = cl_qpool_init(&p_osmt->node_pool, POOL_MIN_ITEMS, 0,
			       POOL_MIN_ITEMS, sizeof(node_t), NULL, NULL,
			       NULL);
	CL_ASSERT(status == CL_SUCCESS);

	status = cl_qpool_init(&p_osmt->port_pool, POOL_MIN_ITEMS, 0,
			       POOL_MIN_ITEMS, sizeof(port_t), NULL, NULL,
			       NULL);
	CL_ASSERT(status == CL_SUCCESS);

	p_osmt->p_vendor = osm_vendor_new(&p_osmt->log,
					  p_opt->transaction_timeout);

	if (p_osmt->p_vendor == NULL) {
		status = IB_INSUFFICIENT_RESOURCES;
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0001: "
			"Unable to allocate vendor object");
		status = IB_ERROR;
		goto Exit;
	}

	osm_mad_pool_construct(&p_osmt->mad_pool);
	status = osm_mad_pool_init(&p_osmt->mad_pool);
	if (status != IB_SUCCESS)
		goto Exit;

Exit:
	OSM_LOG(&p_osmt->log, OSM_LOG_FUNCS, "]\n");
	return (status);
}