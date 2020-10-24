static ib_api_status_t
osmtest_get_path_rec_by_guid_pair(IN osmtest_t * const p_osmt,
				  IN ib_net64_t sguid,
				  IN ib_net64_t dguid,
				  IN osmtest_req_context_t * p_context)
{
	cl_status_t status = IB_SUCCESS;
	osmv_query_req_t req;
	osmv_guid_pair_t guid_pair;

	OSM_LOG_ENTER(&p_osmt->log);

	memset(&req, 0, sizeof(req));
	memset(p_context, 0, sizeof(*p_context));

	p_context->p_osmt = p_osmt;
	req.timeout_ms = p_osmt->opt.transaction_timeout;
	req.retry_cnt = p_osmt->opt.retry_count;
	req.flags = OSM_SA_FLAGS_SYNC;
	req.query_context = p_context;
	req.pfn_query_cb = osmtest_query_res_cb;

	req.query_type = OSMV_QUERY_PATH_REC_BY_PORT_GUIDS;

	guid_pair.dest_guid = dguid;
	guid_pair.src_guid = sguid;

	req.p_query_input = &guid_pair;
	req.sm_key = 0;

	OSM_LOG(&p_osmt->log, OSM_LOG_VERBOSE,
		"Query for path from 0x%" PRIx64 " to 0x%" PRIx64 "\n",
		cl_ntoh64(sguid), cl_ntoh64(dguid));

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