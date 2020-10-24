static void sm_mad_ctrl_process_get_resp(IN osm_sm_mad_ctrl_t * p_ctrl,
					 IN osm_madw_t * p_madw,
					 IN void *transaction_context)
{
	ib_smp_t *p_smp;
	cl_status_t status;
	osm_madw_t *p_old_madw;
	cl_disp_msgid_t msg_id = CL_DISP_MSGID_NONE;

	OSM_LOG_ENTER(p_ctrl->p_log);

	CL_ASSERT(p_madw);
	CL_ASSERT(transaction_context);

	p_smp = osm_madw_get_smp_ptr(p_madw);

	if (p_smp->mgmt_class == IB_MCLASS_SUBN_DIR && !ib_smp_is_d(p_smp)) {
		OSM_LOG(p_ctrl->p_log, OSM_LOG_ERROR, "ERR 3102: "
			"'D' bit not set in returned SMP\n");
		osm_dump_dr_smp_v2(p_ctrl->p_log, p_smp, FILE_ID, OSM_LOG_ERROR);
	}

	p_old_madw = transaction_context;

	sm_mad_ctrl_update_wire_stats(p_ctrl);

	/*
	   Copy the MAD Wrapper context from the requesting MAD
	   to the new MAD.  This mechanism allows the recipient
	   controller to recover its own context regarding this
	   MAD transaction.  Once we've copied the context, we
	   can return the original MAD to the pool.
	 */
	osm_madw_copy_context(p_madw, p_old_madw);
	osm_mad_pool_put(p_ctrl->p_mad_pool, p_old_madw);

	/*
	   Note that attr_id (like the rest of the MAD) is in
	   network byte order.
	 */
	switch (p_smp->attr_id) {
	case IB_MAD_ATTR_NODE_DESC:
		msg_id = OSM_MSG_MAD_NODE_DESC;
		break;
	case IB_MAD_ATTR_NODE_INFO:
		msg_id = OSM_MSG_MAD_NODE_INFO;
		break;
	case IB_MAD_ATTR_GUID_INFO:
		msg_id = OSM_MSG_MAD_GUID_INFO;
		break;
	case IB_MAD_ATTR_SWITCH_INFO:
		msg_id = OSM_MSG_MAD_SWITCH_INFO;
		break;
	case IB_MAD_ATTR_PORT_INFO:
		msg_id = OSM_MSG_MAD_PORT_INFO;
		break;
	case IB_MAD_ATTR_LIN_FWD_TBL:
		msg_id = OSM_MSG_MAD_LFT;
		break;
	case IB_MAD_ATTR_MCAST_FWD_TBL:
		msg_id = OSM_MSG_MAD_MFT;
		break;
	case IB_MAD_ATTR_SM_INFO:
		msg_id = OSM_MSG_MAD_SM_INFO;
		break;
	case IB_MAD_ATTR_SLVL_TABLE:
		msg_id = OSM_MSG_MAD_SLVL;
		break;
	case IB_MAD_ATTR_VL_ARBITRATION:
		msg_id = OSM_MSG_MAD_VL_ARB;
		break;
	case IB_MAD_ATTR_P_KEY_TABLE:
		msg_id = OSM_MSG_MAD_PKEY;
		break;
	case IB_MAD_ATTR_MLNX_EXTENDED_PORT_INFO:
		msg_id = OSM_MSG_MAD_MLNX_EXT_PORT_INFO;
		break;
	case IB_MAD_ATTR_CLASS_PORT_INFO:
	case IB_MAD_ATTR_NOTICE:
	case IB_MAD_ATTR_INFORM_INFO:
	default:
		cl_atomic_inc(&p_ctrl->p_stats->qp0_mads_rcvd_unknown);
		OSM_LOG(p_ctrl->p_log, OSM_LOG_ERROR, "ERR 3103: "
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
		OSM_LOG(p_ctrl->p_log, OSM_LOG_ERROR, "ERR 3104: "
			"Dispatcher post message failed (%s) for attribute 0x%X (%s)\n",
			CL_STATUS_MSG(status), cl_ntoh16(p_smp->attr_id),
			ib_get_sm_attr_str(p_smp->attr_id));
		goto Exit;
	}

Exit:
	OSM_LOG_EXIT(p_ctrl->p_log);
}