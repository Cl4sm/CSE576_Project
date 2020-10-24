static void sm_mad_ctrl_rcv_callback(IN osm_madw_t * p_madw,
				     IN void *bind_context,
				     IN osm_madw_t * p_req_madw)
{
	osm_sm_mad_ctrl_t *p_ctrl = bind_context;
	ib_smp_t *p_smp;
	ib_net16_t status;

	OSM_LOG_ENTER(p_ctrl->p_log);

	CL_ASSERT(p_madw);

	/*
	   A MAD was received from the wire, possibly in response to a request.
	 */
	cl_atomic_inc(&p_ctrl->p_stats->qp0_mads_rcvd);

	OSM_LOG(p_ctrl->p_log, OSM_LOG_DEBUG, "%u QP0 MADs received\n",
		p_ctrl->p_stats->qp0_mads_rcvd);

	p_smp = osm_madw_get_smp_ptr(p_madw);

	/* if we are closing down simply do nothing */
	if (osm_exit_flag) {
		OSM_LOG(p_ctrl->p_log, OSM_LOG_ERROR,
			"Ignoring received mad - since we are exiting\n");

		osm_dump_dr_smp_v2(p_ctrl->p_log, p_smp, FILE_ID, OSM_LOG_DEBUG);

		/* retire the mad or put it back */
		if (ib_smp_is_response(p_smp)) {
			CL_ASSERT(p_madw->resp_expected == FALSE);
			sm_mad_ctrl_retire_trans_mad(p_ctrl, p_madw);
		} else if (p_madw->resp_expected == TRUE)
			sm_mad_ctrl_retire_trans_mad(p_ctrl, p_madw);
		else
			osm_mad_pool_put(p_ctrl->p_mad_pool, p_madw);

		goto Exit;
	}

	if (OSM_LOG_IS_ACTIVE_V2(p_ctrl->p_log, OSM_LOG_FRAMES))
		osm_dump_dr_smp_v2(p_ctrl->p_log, p_smp, FILE_ID, OSM_LOG_FRAMES);

	if (p_smp->mgmt_class == IB_MCLASS_SUBN_DIR)
		status = ib_smp_get_status(p_smp);
	else
		status = p_smp->status;

	if (status != 0)
		log_rcv_cb_error(p_ctrl->p_log, p_smp, status);

	switch (p_smp->method) {
	case IB_MAD_METHOD_GET_RESP:
		CL_ASSERT(p_req_madw != NULL);
		sm_mad_ctrl_process_get_resp(p_ctrl, p_madw, p_req_madw);
		break;
	case IB_MAD_METHOD_GET:
		CL_ASSERT(p_req_madw == NULL);
		sm_mad_ctrl_process_get(p_ctrl, p_madw);
		break;
	case IB_MAD_METHOD_TRAP:
		CL_ASSERT(p_req_madw == NULL);
		sm_mad_ctrl_process_trap(p_ctrl, p_madw);
		break;
	case IB_MAD_METHOD_SET:
		CL_ASSERT(p_req_madw == NULL);
		sm_mad_ctrl_process_set(p_ctrl, p_madw);
		break;
	case IB_MAD_METHOD_TRAP_REPRESS:
		CL_ASSERT(p_req_madw != NULL);
		sm_mad_ctrl_process_trap_repress(p_ctrl, p_madw);
		break;
	case IB_MAD_METHOD_SEND:
	case IB_MAD_METHOD_REPORT:
	case IB_MAD_METHOD_REPORT_RESP:
	default:
		cl_atomic_inc(&p_ctrl->p_stats->qp0_mads_rcvd_unknown);
		OSM_LOG(p_ctrl->p_log, OSM_LOG_ERROR, "ERR 3112: "
			"Unsupported method = 0x%X\n", p_smp->method);
		osm_dump_dr_smp_v2(p_ctrl->p_log, p_smp, FILE_ID, OSM_LOG_ERROR);
		osm_mad_pool_put(p_ctrl->p_mad_pool, p_madw);
		goto Exit;
	}

Exit:
	OSM_LOG_EXIT(p_ctrl->p_log);
}