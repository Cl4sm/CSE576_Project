static ib_api_status_t
osmtest_get_path_rec_by_half_world_query(IN osmtest_t * const p_osmt,
				  IN ib_net64_t sguid,
				  IN osmtest_req_context_t * p_context)
{
	cl_status_t status = IB_SUCCESS;
	osmv_query_req_t req;
	osmv_user_query_t user;
	ib_path_rec_t record;

	OSM_LOG_ENTER(&p_osmt->log);

	memset(&req, 0, sizeof(req));
	memset(p_context, 0, sizeof(*p_context));
	memset(&record, 0, sizeof(record));
	memset(&user, 0, sizeof(user));

        ib_gid_set_default(&(record.sgid), sguid);
	record.num_path = 0x01;

	p_context->p_osmt = p_osmt;
	user.comp_mask = (IB_PR_COMPMASK_SGID | IB_PR_COMPMASK_NUMBPATH);
	user.attr_id = IB_MAD_ATTR_PATH_RECORD;
	user.p_attr = &record;

	req.query_type = OSMV_QUERY_USER_DEFINED;
	req.timeout_ms = p_osmt->opt.transaction_timeout;
	req.retry_cnt = p_osmt->opt.retry_count;
	req.flags = OSM_SA_FLAGS_SYNC;
	req.query_context = p_context;
	req.pfn_query_cb = osmtest_query_res_cb;
	req.p_query_input = &user;
	req.sm_key = 0;

	if (p_osmt->opt.with_grh) {
		req.with_grh = 1;
		memcpy(&req.gid, &p_osmt->sm_port_gid, 16);
	}

	status = osmv_query_sa(p_osmt->h_bind, &req);
	if (status != IB_SUCCESS) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0063: "
			"ib_query failed (%s)\n", ib_get_err_str(status));
		goto Exit;
	}

	status = (*p_context).result.status;

	if (status != IB_SUCCESS) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0066: "
			"ib_query failed (%s)\n", ib_get_err_str(status));

		if (status == IB_REMOTE_ERROR) {
			OSM_LOG(&p_osmt->log, OSM_LOG_ERROR,
				"Remote error = %s\n",
				ib_get_mad_status_str(osm_madw_get_mad_ptr
						      ((*p_context).result.
						       p_result_madw)));
		}
		goto Exit;
	}

Exit:

	OSM_LOG_EXIT(&p_osmt->log);
	return (status);
}