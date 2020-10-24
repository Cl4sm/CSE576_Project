static ib_api_status_t
osmtest_validate_single_node_rec_lid(IN osmtest_t * const p_osmt,
				     IN ib_net16_t const lid,
				     IN node_t * const p_node)
{
	cl_status_t status = IB_SUCCESS;
	osmv_user_query_t user;
	osmv_query_req_t req;
	ib_node_record_t record;

	osmtest_req_context_t context;
	const ib_node_record_t *p_rec;
	int num_recs, i;

	OSM_LOG_ENTER(&p_osmt->log);

	OSM_LOG(&p_osmt->log, OSM_LOG_DEBUG,
		"Getting NodeRecord for node with LID 0x%X\n", cl_ntoh16(lid));

	memset(&context, 0, sizeof(context));
	memset(&req, 0, sizeof(req));
	memset(&user, 0, sizeof(user));
	memset(&record, 0, sizeof(record));

	record.lid = lid;

	context.p_osmt = p_osmt;
	user.comp_mask = IB_NR_COMPMASK_LID;
	user.attr_id = IB_MAD_ATTR_NODE_RECORD;
	user.p_attr = &record;

	req.query_type = OSMV_QUERY_USER_DEFINED;
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
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0105: "
			"ib_query failed (%s)\n", ib_get_err_str(status));
		goto Exit;
	}

	status = context.result.status;

	if (status != IB_SUCCESS) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0106: "
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

	num_recs = context.result.result_cnt;
	OSM_LOG(&p_osmt->log, OSM_LOG_VERBOSE,
		"Received %d nodes\n", num_recs);

	for (i = 0; i < num_recs; i++) {
		p_rec =
		    osmv_get_query_node_rec(context.result.p_result_madw, i);

		status = osmtest_validate_node_rec(p_osmt, p_rec);
		if (status != IB_SUCCESS) {
			OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0107: "
				"osmtest_validate_node_data failed (%s)\n",
				ib_get_err_str(status));
			goto Exit;
		}
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