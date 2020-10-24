ib_api_status_t osm_sa_mad_ctrl_bind(IN osm_sa_mad_ctrl_t * p_ctrl,
				     IN ib_net64_t port_guid)
{
	osm_bind_info_t bind_info;
	ib_api_status_t status = IB_SUCCESS;

	OSM_LOG_ENTER(p_ctrl->p_log);

	if (p_ctrl->h_bind != OSM_BIND_INVALID_HANDLE) {
		OSM_LOG(p_ctrl->p_log, OSM_LOG_ERROR, "ERR 1A09: "
			"Multiple binds not allowed\n");
		status = IB_ERROR;
		goto Exit;
	}

	bind_info.class_version = 2;
	bind_info.is_responder = TRUE;
	bind_info.is_report_processor = FALSE;
	bind_info.is_trap_processor = FALSE;
	bind_info.mad_class = IB_MCLASS_SUBN_ADM;
	bind_info.port_guid = port_guid;
	bind_info.recv_q_size = OSM_SM_DEFAULT_QP1_RCV_SIZE;
	bind_info.send_q_size = OSM_SM_DEFAULT_QP1_SEND_SIZE;
	bind_info.timeout = p_ctrl->sa->p_subn->opt.transaction_timeout;
	bind_info.retries = p_ctrl->sa->p_subn->opt.transaction_retries;

	OSM_LOG(p_ctrl->p_log, OSM_LOG_VERBOSE,
		"Binding to port GUID 0x%" PRIx64 "\n", cl_ntoh64(port_guid));

	p_ctrl->h_bind = osm_vendor_bind(p_ctrl->p_vendor, &bind_info,
					 p_ctrl->p_mad_pool,
					 sa_mad_ctrl_rcv_callback,
					 sa_mad_ctrl_send_err_callback, p_ctrl);

	if (p_ctrl->h_bind == OSM_BIND_INVALID_HANDLE) {
		status = IB_ERROR;
		OSM_LOG(p_ctrl->p_log, OSM_LOG_ERROR, "ERR 1A10: "
			"Vendor specific bind failed (%s)\n",
			ib_get_err_str(status));
		goto Exit;
	}

Exit:
	OSM_LOG_EXIT(p_ctrl->p_log);
	return status;
}