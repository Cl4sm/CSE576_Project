static void sa_mad_ctrl_process(IN osm_sa_mad_ctrl_t * p_ctrl,
				IN osm_madw_t * p_madw)
{
	ib_sa_mad_t *p_sa_mad;
	cl_status_t status;
	cl_disp_msgid_t msg_id = CL_DISP_MSGID_NONE;
	uint64_t last_dispatched_msg_queue_time_msec;
	uint32_t num_messages;

	OSM_LOG_ENTER(p_ctrl->p_log);

	/*
	   If the dispatcher is showing us that it is overloaded
	   there is no point in placing the request in. We should instead
	   provide immediate response - IB_RESOURCE_BUSY
	   But how do we know?
	   The dispatcher reports back the number of outstanding messages and
	   the time the last message stayed in the queue.
	   HACK: Actually, we cannot send a mad from within the receive callback;
	   thus - we will just drop it.
	 */
	cl_disp_get_queue_status(p_ctrl->h_disp, &num_messages,
				 &last_dispatched_msg_queue_time_msec);
	if (num_messages > 1 && p_ctrl->p_subn->opt.max_msg_fifo_timeout &&
	    last_dispatched_msg_queue_time_msec >
	    p_ctrl->p_subn->opt.max_msg_fifo_timeout) {
		OSM_LOG(p_ctrl->p_log, OSM_LOG_INFO,
			/*             "Responding BUSY status since the dispatcher is already" */
			"Dropping MAD since the dispatcher is already"
			" overloaded with %u messages and queue time of:"
			"%" PRIu64 "[msec]\n",
			num_messages, last_dispatched_msg_queue_time_msec);

		/* send a busy response */
		/* osm_sa_send_error(p_ctrl->p_resp, p_madw, IB_RESOURCE_BUSY); */

		/* return the request to the pool */
		osm_mad_pool_put(p_ctrl->p_mad_pool, p_madw);

		goto Exit;
	}

	p_sa_mad = osm_madw_get_sa_mad_ptr(p_madw);

	/*
	   Note that attr_id (like the rest of the MAD) is in
	   network byte order.
	 */
	switch (p_sa_mad->attr_id) {
	case IB_MAD_ATTR_CLASS_PORT_INFO:
		msg_id = OSM_MSG_MAD_CLASS_PORT_INFO;
		break;

	case IB_MAD_ATTR_NODE_RECORD:
		msg_id = OSM_MSG_MAD_NODE_RECORD;
		break;

	case IB_MAD_ATTR_PORTINFO_RECORD:
		msg_id = OSM_MSG_MAD_PORTINFO_RECORD;
		break;

	case IB_MAD_ATTR_LINK_RECORD:
		msg_id = OSM_MSG_MAD_LINK_RECORD;
		break;

	case IB_MAD_ATTR_SMINFO_RECORD:
		msg_id = OSM_MSG_MAD_SMINFO_RECORD;
		break;

	case IB_MAD_ATTR_SERVICE_RECORD:
		msg_id = OSM_MSG_MAD_SERVICE_RECORD;
		break;

	case IB_MAD_ATTR_PATH_RECORD:
		msg_id = OSM_MSG_MAD_PATH_RECORD;
		break;

	case IB_MAD_ATTR_MCMEMBER_RECORD:
		msg_id = OSM_MSG_MAD_MCMEMBER_RECORD;
		break;

	case IB_MAD_ATTR_INFORM_INFO:
		msg_id = OSM_MSG_MAD_INFORM_INFO;
		break;

	case IB_MAD_ATTR_VLARB_RECORD:
		msg_id = OSM_MSG_MAD_VL_ARB_RECORD;
		break;

	case IB_MAD_ATTR_SLVL_RECORD:
		msg_id = OSM_MSG_MAD_SLVL_TBL_RECORD;
		break;

	case IB_MAD_ATTR_PKEY_TBL_RECORD:
		msg_id = OSM_MSG_MAD_PKEY_TBL_RECORD;
		break;

	case IB_MAD_ATTR_LFT_RECORD:
		msg_id = OSM_MSG_MAD_LFT_RECORD;
		break;

	case IB_MAD_ATTR_GUIDINFO_RECORD:
		msg_id = OSM_MSG_MAD_GUIDINFO_RECORD;
		break;

	case IB_MAD_ATTR_INFORM_INFO_RECORD:
		msg_id = OSM_MSG_MAD_INFORM_INFO_RECORD;
		break;

	case IB_MAD_ATTR_SWITCH_INFO_RECORD:
		msg_id = OSM_MSG_MAD_SWITCH_INFO_RECORD;
		break;

	case IB_MAD_ATTR_MFT_RECORD:
		msg_id = OSM_MSG_MAD_MFT_RECORD;
		break;

#if defined (VENDOR_RMPP_SUPPORT) && defined (DUAL_SIDED_RMPP)
	case IB_MAD_ATTR_MULTIPATH_RECORD:
		msg_id = OSM_MSG_MAD_MULTIPATH_RECORD;
		break;
#endif

	default:
		OSM_LOG(p_ctrl->p_log, OSM_LOG_ERROR, "ERR 1A01: "
			"Unsupported attribute 0x%X (%s)\n",
			cl_ntoh16(p_sa_mad->attr_id),
			ib_get_sa_attr_str(p_sa_mad->attr_id));
		osm_dump_sa_mad_v2(p_ctrl->p_log, p_sa_mad, FILE_ID, OSM_LOG_ERROR);
	}

	if (msg_id != CL_DISP_MSGID_NONE) {
		/*
		   Post this MAD to the dispatcher for asynchronous
		   processing by the appropriate controller.
		 */

		OSM_LOG(p_ctrl->p_log, OSM_LOG_DEBUG,
			"Posting Dispatcher message %s\n",
			osm_get_disp_msg_str(msg_id));

		status = cl_disp_post(p_ctrl->h_disp, msg_id, p_madw,
				      sa_mad_ctrl_disp_done_callback, p_ctrl);

		if (status != CL_SUCCESS) {
			OSM_LOG(p_ctrl->p_log, OSM_LOG_ERROR, "ERR 1A02: "
				"Dispatcher post message failed (%s) for attribute 0x%X (%s)\n",
				CL_STATUS_MSG(status),
				cl_ntoh16(p_sa_mad->attr_id),
				ib_get_sa_attr_str(p_sa_mad->attr_id));

			osm_mad_pool_put(p_ctrl->p_mad_pool, p_madw);
			goto Exit;
		}
	} else {
		/*
		   There is an unknown MAD attribute type for which there is
		   no recipient.  Simply retire the MAD here.
		 */
		cl_atomic_inc(&p_ctrl->p_stats->sa_mads_rcvd_unknown);
		osm_mad_pool_put(p_ctrl->p_mad_pool, p_madw);
	}

Exit:
	OSM_LOG_EXIT(p_ctrl->p_log);
}