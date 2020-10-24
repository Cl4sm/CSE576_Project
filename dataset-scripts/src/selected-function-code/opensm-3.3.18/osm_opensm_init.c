ib_api_status_t osm_opensm_init(IN osm_opensm_t * p_osm,
				IN const osm_subn_opt_t * p_opt)
{
	ib_api_status_t status;

	/* Can't use log macros here, since we're initializing the log */
	osm_opensm_construct(p_osm);

	if (p_opt->daemon)
		p_osm->log.daemon = 1;

	status = osm_log_init_v2(&p_osm->log, p_opt->force_log_flush,
				 p_opt->log_flags, p_opt->log_file,
				 p_opt->log_max_size, p_opt->accum_log_file);
	if (status != IB_SUCCESS)
		return status;
	p_osm->log.log_prefix = p_opt->log_prefix;

	/* If there is a log level defined - add the OSM_VERSION to it */
	osm_log_v2(&p_osm->log,
		   osm_log_get_level(&p_osm->log) & (OSM_LOG_SYS ^ 0xFF),
		   FILE_ID, "%s\n", p_osm->osm_version);
	/* Write the OSM_VERSION to the SYS_LOG */
	osm_log_v2(&p_osm->log, OSM_LOG_SYS, FILE_ID, "%s\n", p_osm->osm_version);	/* Format Waived */

	OSM_LOG(&p_osm->log, OSM_LOG_FUNCS, "[\n");	/* Format Waived */

	status = cl_plock_init(&p_osm->lock);
	if (status != IB_SUCCESS)
		goto Exit;

#ifdef HAVE_LIBPTHREAD
	pthread_mutex_init(&p_osm->stats.mutex, NULL);
	pthread_cond_init(&p_osm->stats.cond, NULL);
#else
	status = cl_event_init(&p_osm->stats.event, FALSE);
	if (status != IB_SUCCESS)
		goto Exit;
#endif

	if (p_opt->single_thread) {
		OSM_LOG(&p_osm->log, OSM_LOG_INFO,
			"Forcing single threaded dispatcher\n");
		status = cl_disp_init(&p_osm->disp, 1, "opensm");
	} else {
		/*
		 * Normal behavior is to initialize the dispatcher with
		 * one thread per CPU, as specified by a thread count of '0'.
		 */
		status = cl_disp_init(&p_osm->disp, 0, "opensm");
	}
	if (status != IB_SUCCESS)
		goto Exit;

	/* the DB is in use by subn so init before */
	status = osm_db_init(&p_osm->db, &p_osm->log);
	if (status != IB_SUCCESS)
		goto Exit;

	status = osm_subn_init(&p_osm->subn, p_osm, p_opt);
	if (status != IB_SUCCESS)
		goto Exit;

	p_osm->p_vendor =
	    osm_vendor_new(&p_osm->log, p_opt->transaction_timeout);
	if (p_osm->p_vendor == NULL)
		status = IB_INSUFFICIENT_RESOURCES;

Exit:
	OSM_LOG(&p_osm->log, OSM_LOG_FUNCS, "]\n");	/* Format Waived */
	return status;
}