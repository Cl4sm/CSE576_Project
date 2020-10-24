static void __osmt_print_all_multicast_records(IN osmtest_t * const p_osmt)
{
	uint32_t i;
	ib_api_status_t status;
	osmv_query_req_t req;
	osmv_user_query_t user;
	osmtest_req_context_t context;
	ib_member_rec_t *mcast_record;

	memset(&context, 0, sizeof(context));
	memset(&req, 0, sizeof(req));
	memset(&user, 0, sizeof(user));

	user.attr_id = IB_MAD_ATTR_MCMEMBER_RECORD;

	req.query_type = OSMV_QUERY_USER_DEFINED;
	req.timeout_ms = p_osmt->opt.transaction_timeout;
	req.retry_cnt = 1;
	req.flags = OSM_SA_FLAGS_SYNC;
	context.p_osmt = p_osmt;
	req.query_context = &context;
	req.pfn_query_cb = osmtest_query_res_cb;
	req.p_query_input = &user;

	if (p_osmt->opt.with_grh) {
		req.with_grh = 1;
		memcpy(&req.gid, &p_osmt->sm_port_gid, 16);
	}
	/* UnTrusted (SMKey of 0)  - get the multicast groups */
	status = osmv_query_sa(p_osmt->h_bind, &req);

	if (status != IB_SUCCESS || context.result.status != IB_SUCCESS) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 02B5: "
			"Failed getting the multicast groups records - %s/%s\n",
			ib_get_err_str(status),
			ib_get_err_str(context.result.status));
		return;
	}

	osm_log(&p_osmt->log, OSM_LOG_INFO,
		"\n                    |------------------------------------------|"
		"\n                    |        Remaining Multicast Groups        |"
		"\n                    |------------------------------------------|\n");

	for (i = 0; i < context.result.result_cnt; i++) {
		mcast_record =
		    osmv_get_query_mc_rec(context.result.p_result_madw, i);
		osm_dump_mc_record(&p_osmt->log, mcast_record, OSM_LOG_INFO);
	}

	/* Trusted - now get the multicast group members */
	req.sm_key = OSM_DEFAULT_SM_KEY;
	status = osmv_query_sa(p_osmt->h_bind, &req);

	if (status != IB_SUCCESS || context.result.status != IB_SUCCESS) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 02B6: "
			"Failed getting the multicast group members records - %s/%s\n",
			ib_get_err_str(status),
			ib_get_err_str(context.result.status));
		return;
	}

	osm_log(&p_osmt->log, OSM_LOG_INFO,
		"\n                    |--------------------------------------------------|"
		"\n                    |        Remaining Multicast Group Members        |"
		"\n                    |--------------------------------------------------|\n");

	for (i = 0; i < context.result.result_cnt; i++) {
		mcast_record =
		    osmv_get_query_mc_rec(context.result.p_result_madw, i);
		osm_dump_mc_record(&p_osmt->log, mcast_record, OSM_LOG_INFO);
	}

}