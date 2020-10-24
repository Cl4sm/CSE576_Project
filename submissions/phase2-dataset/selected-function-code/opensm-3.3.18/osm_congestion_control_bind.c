ib_api_status_t osm_congestion_control_bind(osm_congestion_control_t * p_cc,
					    ib_net64_t port_guid)
{
	osm_bind_info_t bind_info;
	ib_api_status_t status = IB_SUCCESS;

	OSM_LOG_ENTER(p_cc->log);

	bind_info.port_guid = p_cc->port_guid = port_guid;
	bind_info.mad_class = IB_MCLASS_CC;
	bind_info.class_version = 2;
	bind_info.is_responder = FALSE;
	bind_info.is_report_processor = FALSE;
	bind_info.is_trap_processor = FALSE;
	bind_info.recv_q_size = OSM_SM_DEFAULT_QP1_RCV_SIZE;
	bind_info.send_q_size = OSM_SM_DEFAULT_QP1_SEND_SIZE;
	bind_info.timeout = p_cc->subn->opt.transaction_timeout;
	bind_info.retries = p_cc->subn->opt.transaction_retries;

	OSM_LOG(p_cc->log, OSM_LOG_VERBOSE,
		"Binding to port GUID 0x%" PRIx64 "\n", cl_ntoh64(port_guid));

	p_cc->bind_handle = osm_vendor_bind(p_cc->vendor, &bind_info,
					    p_cc->mad_pool,
					    cc_mad_recv_callback,
					    cc_mad_send_err_callback, p_cc);

	if (p_cc->bind_handle == OSM_BIND_INVALID_HANDLE) {
		status = IB_ERROR;
		OSM_LOG(p_cc->log, OSM_LOG_ERROR,
			"ERR C107: Vendor specific bind failed (%s)\n",
			ib_get_err_str(status));
		goto Exit;
	}

Exit:
	OSM_LOG_EXIT(p_cc->log);
	return status;
}