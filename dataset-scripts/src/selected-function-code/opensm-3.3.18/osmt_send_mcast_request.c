ib_api_status_t
osmt_send_mcast_request(IN osmtest_t * const p_osmt,
			IN uint8_t is_set,
			IN ib_member_rec_t * p_mc_req,
			IN uint64_t comp_mask, OUT ib_sa_mad_t * p_res)
{
	osmtest_req_context_t context;
	ib_api_status_t status = IB_SUCCESS;
	osmv_user_query_t user;
	osmv_query_req_t req;

	OSM_LOG_ENTER(&p_osmt->log);

	/*
	 * Do a blocking query for this record in the subnet.
	 *
	 * The query structures are locals.
	 */
	memset(&req, 0, sizeof(req));
	memset(&user, 0, sizeof(user));
	memset(&context, 0, sizeof(context));
	memset(p_res, 0, sizeof(ib_sa_mad_t));

	context.p_osmt = p_osmt;

	user.p_attr = p_mc_req;
	user.comp_mask = comp_mask;

	if (is_set == 1)
		req.query_type = OSMV_QUERY_UD_MULTICAST_SET;
	else if (is_set == 0)
		req.query_type = OSMV_QUERY_UD_MULTICAST_DELETE;
	else if (is_set == 0xee) {
		OSM_LOG(&p_osmt->log, OSM_LOG_VERBOSE,
			"Set USER DEFINED QUERY\n");
		req.query_type = OSMV_QUERY_USER_DEFINED;
		user.method = IB_MAD_METHOD_GET;
		user.attr_id = IB_MAD_ATTR_MCMEMBER_RECORD;
	} else if (is_set == 0xff) {
		OSM_LOG(&p_osmt->log, OSM_LOG_VERBOSE,
			"Set USER DEFINED QUERY\n");
		req.query_type = OSMV_QUERY_USER_DEFINED;
		user.method = IB_MAD_METHOD_SET;
		user.attr_id = IB_MAD_ATTR_MCMEMBER_RECORD;
	}

	/* TODO : Check the validity of all user fields in order to use
	   OSMV_QUERY_USER_DEFINED
	   p_user_query = ( osmv_user_query_t * ) p_query_req->p_query_input;
	   if (p_user_query->method) sa_mad_data.method = p_user_query->method;
	   sa_mad_data.attr_offset = p_user_query->attr_offset;
	   sa_mad_data.attr_id = p_user_query->attr_id;
	   sa_mad_data.comp_mask = p_user_query->comp_mask;
	   sa_mad_data.p_attr = p_user_query->p_attr;
	 */

	req.timeout_ms = p_osmt->opt.transaction_timeout;
	req.retry_cnt = p_osmt->opt.retry_count;
	req.flags = OSM_SA_FLAGS_SYNC;
	req.query_context = &context;
	req.pfn_query_cb = osmtest_query_res_cb;
	req.p_query_input = &user;

	if (p_osmt->opt.with_grh) {
		req.with_grh = 1;
		memcpy(&req.gid, &p_osmt->sm_port_gid, 16);
	}

	status = osmv_query_sa(p_osmt->h_bind, &req);

	if (status != IB_SUCCESS) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0206: "
			"ib_query failed (%s)\n", ib_get_err_str(status));
		goto Exit;
	}

	/* ok it worked */
	memcpy(p_res, osm_madw_get_mad_ptr(context.result.p_result_madw),
	       sizeof(ib_sa_mad_t));

	status = context.result.status;

	if (status != IB_SUCCESS) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0224: "
			"ib_query failed (%s)\n", ib_get_err_str(status));
		if (status == IB_REMOTE_ERROR)
			OSM_LOG(&p_osmt->log, OSM_LOG_ERROR,
				"Remote error = %s\n",
				ib_get_mad_status_str(osm_madw_get_mad_ptr
						      (context.result.
						       p_result_madw)));
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