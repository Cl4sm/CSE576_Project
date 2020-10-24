ib_api_status_t osm_sm_init(IN osm_sm_t * p_sm, IN osm_subn_t * p_subn,
			    IN osm_db_t * p_db, IN osm_vendor_t * p_vendor,
			    IN osm_mad_pool_t * p_mad_pool,
			    IN osm_vl15_t * p_vl15, IN osm_log_t * p_log,
			    IN osm_stats_t * p_stats,
			    IN cl_dispatcher_t * p_disp, IN cl_plock_t * p_lock)
{
	ib_api_status_t status;

	OSM_LOG_ENTER(p_log);

	p_sm->p_subn = p_subn;
	p_sm->p_db = p_db;
	p_sm->p_vendor = p_vendor;
	p_sm->p_mad_pool = p_mad_pool;
	p_sm->p_vl15 = p_vl15;
	p_sm->p_log = p_log;
	p_sm->p_disp = p_disp;
	p_sm->p_lock = p_lock;

	status = cl_spinlock_init(&p_sm->signal_lock);
	if (status != CL_SUCCESS)
		goto Exit;

	status = cl_spinlock_init(&p_sm->state_lock);
	if (status != CL_SUCCESS)
		goto Exit;

	status = cl_event_init(&p_sm->signal_event, FALSE);
	if (status != CL_SUCCESS)
		goto Exit;

	status = cl_event_init(&p_sm->subnet_up_event, FALSE);
	if (status != CL_SUCCESS)
		goto Exit;

	status = cl_timer_init(&p_sm->sweep_timer, sm_sweep, p_sm);
	if (status != CL_SUCCESS)
		goto Exit;

	status = cl_timer_init(&p_sm->polling_timer,
			       osm_sm_state_mgr_polling_callback, p_sm);
	if (status != CL_SUCCESS)
		goto Exit;

	p_sm->mlids_req_max = 0;
	p_sm->mlids_req = malloc((IB_LID_MCAST_END_HO - IB_LID_MCAST_START_HO +
				  1) * sizeof(p_sm->mlids_req[0]));
	if (!p_sm->mlids_req)
		goto Exit;
	memset(p_sm->mlids_req, 0,
	       (IB_LID_MCAST_END_HO - IB_LID_MCAST_START_HO +
		1) * sizeof(p_sm->mlids_req[0]));

	status = osm_sm_mad_ctrl_init(&p_sm->mad_ctrl, p_sm->p_subn,
				      p_sm->p_mad_pool, p_sm->p_vl15,
				      p_sm->p_vendor,
				      p_log, p_stats, p_lock, p_disp);
	if (status != IB_SUCCESS)
		goto Exit;

	status = cl_event_wheel_init(&p_sm->trap_aging_tracker);
	if (status != IB_SUCCESS)
		goto Exit;

	status = osm_lid_mgr_init(&p_sm->lid_mgr, p_sm);
	if (status != IB_SUCCESS)
		goto Exit;

	status = osm_ucast_mgr_init(&p_sm->ucast_mgr, p_sm);
	if (status != IB_SUCCESS)
		goto Exit;

	status = IB_INSUFFICIENT_RESOURCES;
	p_sm->sweep_fail_disp_h = cl_disp_register(p_disp,
						   OSM_MSG_LIGHT_SWEEP_FAIL,
						   sweep_fail_process, p_sm);
	if (p_sm->sweep_fail_disp_h == CL_DISP_INVALID_HANDLE)
		goto Exit;

	p_sm->ni_disp_h = cl_disp_register(p_disp, OSM_MSG_MAD_NODE_INFO,
					   osm_ni_rcv_process, p_sm);
	if (p_sm->ni_disp_h == CL_DISP_INVALID_HANDLE)
		goto Exit;

	p_sm->pi_disp_h = cl_disp_register(p_disp, OSM_MSG_MAD_PORT_INFO,
					   osm_pi_rcv_process, p_sm);
	if (p_sm->pi_disp_h == CL_DISP_INVALID_HANDLE)
		goto Exit;

	p_sm->gi_disp_h = cl_disp_register(p_disp, OSM_MSG_MAD_GUID_INFO,
					   osm_gi_rcv_process, p_sm);
	if (p_sm->gi_disp_h == CL_DISP_INVALID_HANDLE)
		goto Exit;

	p_sm->si_disp_h = cl_disp_register(p_disp, OSM_MSG_MAD_SWITCH_INFO,
					   osm_si_rcv_process, p_sm);
	if (p_sm->si_disp_h == CL_DISP_INVALID_HANDLE)
		goto Exit;

	p_sm->nd_disp_h = cl_disp_register(p_disp, OSM_MSG_MAD_NODE_DESC,
					   osm_nd_rcv_process, p_sm);
	if (p_sm->nd_disp_h == CL_DISP_INVALID_HANDLE)
		goto Exit;

	p_sm->lft_disp_h = cl_disp_register(p_disp, OSM_MSG_MAD_LFT,
					    osm_lft_rcv_process, p_sm);
	if (p_sm->lft_disp_h == CL_DISP_INVALID_HANDLE)
		goto Exit;

	p_sm->mft_disp_h = cl_disp_register(p_disp, OSM_MSG_MAD_MFT,
					    osm_mft_rcv_process, p_sm);
	if (p_sm->mft_disp_h == CL_DISP_INVALID_HANDLE)
		goto Exit;

	p_sm->sm_info_disp_h = cl_disp_register(p_disp, OSM_MSG_MAD_SM_INFO,
						osm_sminfo_rcv_process, p_sm);
	if (p_sm->sm_info_disp_h == CL_DISP_INVALID_HANDLE)
		goto Exit;

	p_sm->trap_disp_h = cl_disp_register(p_disp, OSM_MSG_MAD_NOTICE,
					     osm_trap_rcv_process, p_sm);
	if (p_sm->trap_disp_h == CL_DISP_INVALID_HANDLE)
		goto Exit;

	p_sm->slvl_disp_h = cl_disp_register(p_disp, OSM_MSG_MAD_SLVL,
					     osm_slvl_rcv_process, p_sm);
	if (p_sm->slvl_disp_h == CL_DISP_INVALID_HANDLE)
		goto Exit;

	p_sm->vla_disp_h = cl_disp_register(p_disp, OSM_MSG_MAD_VL_ARB,
					    osm_vla_rcv_process, p_sm);
	if (p_sm->vla_disp_h == CL_DISP_INVALID_HANDLE)
		goto Exit;

	p_sm->pkey_disp_h = cl_disp_register(p_disp, OSM_MSG_MAD_PKEY,
					     osm_pkey_rcv_process, p_sm);
	if (p_sm->pkey_disp_h == CL_DISP_INVALID_HANDLE)
		goto Exit;

	p_sm->mlnx_epi_disp_h = cl_disp_register(p_disp,
						 OSM_MSG_MAD_MLNX_EXT_PORT_INFO,
						 osm_mlnx_epi_rcv_process, p_sm);
	if (p_sm->mlnx_epi_disp_h == CL_DISP_INVALID_HANDLE)
		goto Exit;

	p_subn->sm_state = p_subn->opt.sm_inactive ?
	    IB_SMINFO_STATE_NOTACTIVE : IB_SMINFO_STATE_DISCOVERING;
	osm_report_sm_state(p_sm);

	/*
	 * Now that the component objects are initialized, start
	 * the sweeper thread if the user wants sweeping.
	 */
	p_sm->thread_state = OSM_THREAD_STATE_RUN;
	status = cl_thread_init(&p_sm->sweeper, sm_sweeper, p_sm,
				"opensm sweeper");
	if (status != IB_SUCCESS)
		goto Exit;

	if (p_sm->p_subn->opt.sweep_interval)
		cl_timer_start(&p_sm->sweep_timer,
			       p_sm->p_subn->opt.sweep_interval * 1000);

Exit:
	OSM_LOG_EXIT(p_log);
	return status;
}