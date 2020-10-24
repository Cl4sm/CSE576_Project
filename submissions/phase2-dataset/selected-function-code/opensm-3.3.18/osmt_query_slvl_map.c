ib_api_status_t
osmt_query_slvl_map(IN osmtest_t * const p_osmt,
		    IN ib_net16_t const lid,
		    IN uint8_t const out_port_num,
		    IN uint8_t const in_port_num, IN FILE * fh)
{
	osmtest_req_context_t context;
	ib_api_status_t status = IB_SUCCESS;
	osmv_user_query_t user;
	osmv_query_req_t req;
	ib_slvl_table_record_t record, *p_rec;

	OSM_LOG_ENTER(&p_osmt->log);

	OSM_LOG(&p_osmt->log, OSM_LOG_DEBUG,
		"Getting SLtoVL Map Table for out-port with LID 0x%X Num:0x%X from In-Port:0x%X\n",
		cl_ntoh16(lid), out_port_num, in_port_num);

	/*
	 * Do a blocking query for this record in the subnet.
	 * The result is returned in the result field of the caller's
	 * context structure.
	 *
	 * The query structures are locals.
	 */
	memset(&req, 0, sizeof(req));
	memset(&user, 0, sizeof(user));
	memset(&context, 0, sizeof(context));

	context.p_osmt = p_osmt;

	record.lid = lid;
	record.in_port_num = in_port_num;
	record.out_port_num = out_port_num;
	user.p_attr = &record;

	req.query_type = OSMV_QUERY_SLVL_BY_LID_AND_PORTS;
	req.timeout_ms = p_osmt->opt.transaction_timeout;
	req.retry_cnt = p_osmt->opt.retry_count;
	req.flags = OSM_SA_FLAGS_SYNC;
	req.query_context = &context;
	req.pfn_query_cb = osmtest_query_res_cb;
	req.p_query_input = &user;
	req.sm_key = 0;

	if (p_osmt->opt.with_grh) {
		req.with_grh = 1;
		memcpy(&req.gid, &p_osmt->sm_port_gid, 16);
	}

	status = osmv_query_sa(p_osmt->h_bind, &req);

	if (status != IB_SUCCESS) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0469: "
			"ib_query failed (%s)\n", ib_get_err_str(status));
		goto Exit;
	}

	status = context.result.status;

	if (status != IB_SUCCESS) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0470: "
			"ib_query failed (%s)\n", ib_get_err_str(status));

		if (status == IB_REMOTE_ERROR) {
			OSM_LOG(&p_osmt->log, OSM_LOG_ERROR,
				"Remote error = %s\n",
				ib_get_mad_status_str(osm_madw_get_mad_ptr
						      (context.result.
						       p_result_madw)));
		}
		goto Exit;
	}

	/* ok it worked */
	p_rec = osmv_get_query_result(context.result.p_result_madw, 0);
	if (fh) {
		osmtest_write_slvl_map_table(p_osmt, fh, p_rec);
	}

Exit:
	/*
	 * Return the IB query MAD to the pool as necessary.
	 */
	if (context.result.p_result_madw != NULL) {
		osm_mad_pool_put(&p_osmt->mad_pool,
				 context.result.p_result_madw);
		context.result.p_result_madw = NULL;
	}

	OSM_LOG_EXIT(&p_osmt->log);
	return (status);
}