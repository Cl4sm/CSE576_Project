static void sm_mad_ctrl_process_trap(IN osm_sm_mad_ctrl_t * p_ctrl,
				     IN osm_madw_t * p_madw)
{
	ib_smp_t *p_smp;
	cl_status_t status;
	cl_disp_msgid_t msg_id = CL_DISP_MSGID_NONE;

	OSM_LOG_ENTER(p_ctrl->p_log);

	p_smp = osm_madw_get_smp_ptr(p_madw);

	/* Make sure OpenSM is master. If not - then we should not process the trap */
	if (p_ctrl->p_subn->sm_state != IB_SMINFO_STATE_MASTER) {
		OSM_LOG(p_ctrl->p_log, OSM_LOG_DEBUG,
			"Received trap but OpenSM is not in MASTER state. "
			"Dropping mad\n");
		osm_mad_pool_put(p_ctrl->p_mad_pool, p_madw);
		goto Exit;
	}

	/*
	   Note that attr_id (like the rest of the MAD) is in
	   network byte order.
	 */
	switch (p_smp->attr_id) {
	case IB_MAD_ATTR_NOTICE:
		msg_id = OSM_MSG_MAD_NOTICE;
		break;
	default:
		cl_atomic_inc(&p_ctrl->p_stats->qp0_mads_rcvd_unknown);
		OSM_LOG(p_ctrl->p_log, OSM_LOG_ERROR, "ERR 3109: "
			"Unsupported attribute 0x%X (%s)\n",
			cl_ntoh16(p_smp->attr_id),
			ib_get_sm_attr_str(p_smp->attr_id));
		osm_dump_dr_smp_v2(p_ctrl->p_log, p_smp, FILE_ID, OSM_LOG_ERROR);
		osm_mad_pool_put(p_ctrl->p_mad_pool, p_madw);
		goto Exit;
	}

	/*
	   Post this MAD to the dispatcher for asynchronous
	   processing by the appropriate controller.
	 */

	OSM_LOG(p_ctrl->p_log, OSM_LOG_DEBUG, "Posting Dispatcher message %s\n",
		osm_get_disp_msg_str(msg_id));

	status = cl_disp_post(p_ctrl->h_disp, msg_id, p_madw,
			      sm_mad_ctrl_disp_done_callback, p_ctrl);

	if (status != CL_SUCCESS) {
		OSM_LOG(p_ctrl->p_log, OSM_LOG_ERROR, "ERR 3110: "
			"Dispatcher post message failed (%s)\n",
			CL_STATUS_MSG(status));
		goto Exit;
	}

Exit:
	OSM_LOG_EXIT(p_ctrl->p_log);
}