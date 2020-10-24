ib_api_status_t osmtest_wrong_sm_key_ignored(IN osmtest_t * const p_osmt)
{
	ib_api_status_t status = IB_SUCCESS;
	osmv_user_query_t user;
	osmv_query_req_t req;
	ib_portinfo_record_t record;
	osmtest_req_context_t context;
	osmtest_req_context_t *p_context = &context;
	uint8_t port_num = 1;

	OSM_LOG_ENTER(&p_osmt->log);

	OSM_LOG(&p_osmt->log, OSM_LOG_INFO,
		"Trying PortInfoRecord for port with LID 0x%X Num:0x%X\n",
		p_osmt->local_port.sm_lid, port_num);

	/*
	 * Do a blocking query for this record in the subnet.
	 * The result is returned in the result field of the caller's
	 * context structure.
	 *
	 * The query structures are locals.
	 */
	memset(&req, 0, sizeof(req));
	memset(&user, 0, sizeof(user));
	memset(&record, 0, sizeof(record));

	record.lid = p_osmt->local_port.sm_lid;
	record.port_num = port_num;
	user.p_attr = &record;

	p_context->p_osmt = p_osmt;

	req.query_type = OSMV_QUERY_PORT_REC_BY_LID_AND_NUM;
	req.timeout_ms = p_osmt->opt.transaction_timeout;
	req.retry_cnt = p_osmt->opt.retry_count;
	req.flags = OSM_SA_FLAGS_SYNC;
	req.query_context = p_context;
	req.pfn_query_cb = osmtest_query_res_cb;
	req.p_query_input = &user;
	req.sm_key = 9999;
	context.result.p_result_madw = NULL;

	if (p_osmt->opt.with_grh) {
		req.with_grh = 1;
		memcpy(&req.gid, &p_osmt->sm_port_gid, 16);
	}

	OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, EXPECTING_ERRORS_START "\n");
	status = osmv_query_sa(p_osmt->h_bind, &req);
	OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, EXPECTING_ERRORS_END "\n");

	/* since we use a wrong sm_key we should get a timeout */
	if (status != IB_TIMEOUT) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0011: "
			"Did not get a timeout but got (%s)\n",
			ib_get_err_str(status));
		if (status == IB_SUCCESS) {
			/* assign some error value to status, since IB_SUCCESS is a bad rc */
			status = IB_ERROR;
		}
		goto Exit;
	} else {
		status = IB_SUCCESS;
	}

Exit:
	if (context.result.p_result_madw != NULL) {
		osm_mad_pool_put(&p_osmt->mad_pool,
				 context.result.p_result_madw);
		context.result.p_result_madw = NULL;
	}

	OSM_LOG_EXIT(&p_osmt->log);
	return (status);
}