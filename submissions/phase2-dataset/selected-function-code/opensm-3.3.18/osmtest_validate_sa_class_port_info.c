ib_api_status_t osmtest_validate_sa_class_port_info(IN osmtest_t * const p_osmt)
{
	ib_api_status_t status = IB_SUCCESS;
	osmv_query_req_t req;
	ib_class_port_info_t *p_cpi;
	osmtest_req_context_t context;
	osmtest_req_context_t *p_context = &context;
	ib_sa_mad_t *p_resp_sa_madp;

	OSM_LOG_ENTER(&p_osmt->log);

	OSM_LOG(&p_osmt->log, OSM_LOG_VERBOSE, "Getting ClassPortInfo\n");

	/*
	 * Do a blocking query for this record in the subnet.
	 * The result is returned in the result field of the caller's
	 * context structure.
	 *
	 * The query structures are locals.
	 */
	memset(&req, 0, sizeof(req));

	p_context->p_osmt = p_osmt;
	req.query_type = OSMV_QUERY_CLASS_PORT_INFO;
	req.timeout_ms = p_osmt->opt.transaction_timeout;
	req.retry_cnt = p_osmt->opt.retry_count;
	req.flags = OSM_SA_FLAGS_SYNC;
	req.query_context = p_context;
	req.pfn_query_cb = osmtest_query_res_cb;
	req.p_query_input = 0;
	req.sm_key = 0;

	if (p_osmt->opt.flow != OSMT_FLOW_CREATE_INVENTORY &&
	    p_osmt->opt.with_grh) {
		req.with_grh = 1;
		memcpy(&req.gid, &p_osmt->sm_port_gid, 16);
	}

	status = osmv_query_sa(p_osmt->h_bind, &req);
	if (status != IB_SUCCESS) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0065: "
			"ib_query failed (%s)\n", ib_get_err_str(status));
		goto Exit;
	}

	status = p_context->result.status;

	if (status != IB_SUCCESS) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0070: "
			"ib_query failed (%s)\n", ib_get_err_str(status));
		if (status == IB_REMOTE_ERROR) {
			OSM_LOG(&p_osmt->log, OSM_LOG_ERROR,
				"Remote error = %s\n",
				ib_get_mad_status_str(osm_madw_get_mad_ptr
						      (p_context->result.
						       p_result_madw)));
		}
		goto Exit;
	}

	/* ok we got it so please print it out */
	p_resp_sa_madp =
	    (ib_sa_mad_t *) osm_madw_get_mad_ptr(context.result.p_result_madw);
	p_cpi =
	    (ib_class_port_info_t *) ib_sa_mad_get_payload_ptr(p_resp_sa_madp);

	OSM_LOG(&p_osmt->log, OSM_LOG_INFO,
		"\n-----------------------------\n"
		"SA Class Port Info:\n"
		" base_ver:%u\n"
		" class_ver:%u\n"
		" cap_mask:0x%X\n"
		" cap_mask2:0x%X\n"
		" resp_time_val:0x%X\n"
		"-----------------------------\n",
		p_cpi->base_ver, p_cpi->class_ver, cl_ntoh16(p_cpi->cap_mask),
		ib_class_cap_mask2(p_cpi), ib_class_resp_time_val(p_cpi));

Exit:
#if 0
	if (context.result.p_result_madw != NULL) {
		osm_mad_pool_put(&p_osmt->mad_pool,
				 context.result.p_result_madw);
		context.result.p_result_madw = NULL;
	}
#endif

	OSM_LOG_EXIT(&p_osmt->log);
	return (status);
}