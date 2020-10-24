ib_api_status_t osm_sa_init(IN osm_sm_t * p_sm, IN osm_sa_t * p_sa,
			    IN osm_subn_t * p_subn, IN osm_vendor_t * p_vendor,
			    IN osm_mad_pool_t * p_mad_pool,
			    IN osm_log_t * p_log, IN osm_stats_t * p_stats,
			    IN cl_dispatcher_t * p_disp, IN cl_plock_t * p_lock)
{
	ib_api_status_t status;

	OSM_LOG_ENTER(p_log);

	p_sa->sm = p_sm;
	p_sa->p_subn = p_subn;
	p_sa->p_vendor = p_vendor;
	p_sa->p_mad_pool = p_mad_pool;
	p_sa->p_log = p_log;
	p_sa->p_disp = p_disp;
	p_sa->p_lock = p_lock;

	p_sa->state = OSM_SA_STATE_READY;

	status = osm_sa_mad_ctrl_init(&p_sa->mad_ctrl, p_sa, p_sa->p_mad_pool,
				      p_sa->p_vendor, p_subn, p_log, p_stats,
				      p_disp);
	if (status != IB_SUCCESS)
		goto Exit;

	status = cl_timer_init(&p_sa->sr_timer, osm_sr_rcv_lease_cb, p_sa);
	if (status != IB_SUCCESS)
		goto Exit;

	status = IB_INSUFFICIENT_RESOURCES;
	p_sa->cpi_disp_h = cl_disp_register(p_disp, OSM_MSG_MAD_CLASS_PORT_INFO,
					    osm_cpi_rcv_process, p_sa);
	if (p_sa->cpi_disp_h == CL_DISP_INVALID_HANDLE)
		goto Exit;

	p_sa->nr_disp_h = cl_disp_register(p_disp, OSM_MSG_MAD_NODE_RECORD,
					   osm_nr_rcv_process, p_sa);
	if (p_sa->nr_disp_h == CL_DISP_INVALID_HANDLE)
		goto Exit;

	p_sa->pir_disp_h = cl_disp_register(p_disp, OSM_MSG_MAD_PORTINFO_RECORD,
					    osm_pir_rcv_process, p_sa);
	if (p_sa->pir_disp_h == CL_DISP_INVALID_HANDLE)
		goto Exit;

	p_sa->gir_disp_h = cl_disp_register(p_disp, OSM_MSG_MAD_GUIDINFO_RECORD,
					    osm_gir_rcv_process, p_sa);
	if (p_sa->gir_disp_h == CL_DISP_INVALID_HANDLE)
		goto Exit;

	p_sa->lr_disp_h = cl_disp_register(p_disp, OSM_MSG_MAD_LINK_RECORD,
					   osm_lr_rcv_process, p_sa);
	if (p_sa->lr_disp_h == CL_DISP_INVALID_HANDLE)
		goto Exit;

	p_sa->pr_disp_h = cl_disp_register(p_disp, OSM_MSG_MAD_PATH_RECORD,
					   osm_pr_rcv_process, p_sa);
	if (p_sa->pr_disp_h == CL_DISP_INVALID_HANDLE)
		goto Exit;

#if defined (VENDOR_RMPP_SUPPORT) && defined (DUAL_SIDED_RMPP)
	p_sa->mpr_disp_h =
	    cl_disp_register(p_disp, OSM_MSG_MAD_MULTIPATH_RECORD,
			     osm_mpr_rcv_process, p_sa);
	if (p_sa->mpr_disp_h == CL_DISP_INVALID_HANDLE)
		goto Exit;
#endif

	p_sa->smir_disp_h = cl_disp_register(p_disp, OSM_MSG_MAD_SMINFO_RECORD,
					     osm_smir_rcv_process, p_sa);
	if (p_sa->smir_disp_h == CL_DISP_INVALID_HANDLE)
		goto Exit;

	p_sa->mcmr_disp_h =
	    cl_disp_register(p_disp, OSM_MSG_MAD_MCMEMBER_RECORD,
			     osm_mcmr_rcv_process, p_sa);
	if (p_sa->mcmr_disp_h == CL_DISP_INVALID_HANDLE)
		goto Exit;

	p_sa->sr_disp_h = cl_disp_register(p_disp, OSM_MSG_MAD_SERVICE_RECORD,
					   osm_sr_rcv_process, p_sa);
	if (p_sa->sr_disp_h == CL_DISP_INVALID_HANDLE)
		goto Exit;

	p_sa->infr_disp_h = cl_disp_register(p_disp, OSM_MSG_MAD_INFORM_INFO,
					     osm_infr_rcv_process, p_sa);
	if (p_sa->infr_disp_h == CL_DISP_INVALID_HANDLE)
		goto Exit;

	p_sa->infir_disp_h =
	    cl_disp_register(p_disp, OSM_MSG_MAD_INFORM_INFO_RECORD,
			     osm_infir_rcv_process, p_sa);
	if (p_sa->infir_disp_h == CL_DISP_INVALID_HANDLE)
		goto Exit;

	p_sa->vlarb_disp_h = cl_disp_register(p_disp, OSM_MSG_MAD_VL_ARB_RECORD,
					      osm_vlarb_rec_rcv_process, p_sa);
	if (p_sa->vlarb_disp_h == CL_DISP_INVALID_HANDLE)
		goto Exit;

	p_sa->slvl_disp_h =
	    cl_disp_register(p_disp, OSM_MSG_MAD_SLVL_TBL_RECORD,
			     osm_slvl_rec_rcv_process, p_sa);
	if (p_sa->slvl_disp_h == CL_DISP_INVALID_HANDLE)
		goto Exit;

	p_sa->pkey_disp_h =
	    cl_disp_register(p_disp, OSM_MSG_MAD_PKEY_TBL_RECORD,
			     osm_pkey_rec_rcv_process, p_sa);
	if (p_sa->pkey_disp_h == CL_DISP_INVALID_HANDLE)
		goto Exit;

	p_sa->lft_disp_h = cl_disp_register(p_disp, OSM_MSG_MAD_LFT_RECORD,
					    osm_lftr_rcv_process, p_sa);
	if (p_sa->lft_disp_h == CL_DISP_INVALID_HANDLE)
		goto Exit;

	p_sa->sir_disp_h =
	    cl_disp_register(p_disp, OSM_MSG_MAD_SWITCH_INFO_RECORD,
			     osm_sir_rcv_process, p_sa);
	if (p_sa->sir_disp_h == CL_DISP_INVALID_HANDLE)
		goto Exit;

	p_sa->mft_disp_h = cl_disp_register(p_disp, OSM_MSG_MAD_MFT_RECORD,
					    osm_mftr_rcv_process, p_sa);
	if (p_sa->mft_disp_h == CL_DISP_INVALID_HANDLE)
		goto Exit;

	status = IB_SUCCESS;
Exit:
	OSM_LOG_EXIT(p_log);
	return status;
}