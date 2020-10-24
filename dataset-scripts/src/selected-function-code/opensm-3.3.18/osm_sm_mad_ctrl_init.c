ib_api_status_t osm_sm_mad_ctrl_init(IN osm_sm_mad_ctrl_t * p_ctrl,
				     IN osm_subn_t * p_subn,
				     IN osm_mad_pool_t * p_mad_pool,
				     IN osm_vl15_t * p_vl15,
				     IN osm_vendor_t * p_vendor,
				     IN osm_log_t * p_log,
				     IN osm_stats_t * p_stats,
				     IN cl_plock_t * p_lock,
				     IN cl_dispatcher_t * p_disp)
{
	ib_api_status_t status = IB_SUCCESS;

	OSM_LOG_ENTER(p_log);

	osm_sm_mad_ctrl_construct(p_ctrl);

	p_ctrl->p_subn = p_subn;
	p_ctrl->p_log = p_log;
	p_ctrl->p_disp = p_disp;
	p_ctrl->p_mad_pool = p_mad_pool;
	p_ctrl->p_vendor = p_vendor;
	p_ctrl->p_stats = p_stats;
	p_ctrl->p_lock = p_lock;
	p_ctrl->p_vl15 = p_vl15;

	p_ctrl->h_disp = cl_disp_register(p_disp, CL_DISP_MSGID_NONE, NULL,
					  NULL);

	if (p_ctrl->h_disp == CL_DISP_INVALID_HANDLE) {
		OSM_LOG(p_log, OSM_LOG_ERROR, "ERR 3116: "
			"Dispatcher registration failed\n");
		status = IB_INSUFFICIENT_RESOURCES;
		goto Exit;
	}

Exit:
	OSM_LOG_EXIT(p_log);
	return status;
}