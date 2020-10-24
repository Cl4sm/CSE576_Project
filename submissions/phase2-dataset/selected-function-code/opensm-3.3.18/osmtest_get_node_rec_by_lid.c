ib_api_status_t
osmtest_get_node_rec_by_lid(IN osmtest_t * const p_osmt,
			    IN ib_net16_t const lid,
			    IN OUT osmtest_req_context_t * const p_context)
{
	ib_api_status_t status = IB_SUCCESS;
	osmv_user_query_t user;
	osmv_query_req_t req;
	ib_node_record_t record;
	ib_mad_t *p_mad;

	OSM_LOG_ENTER(&p_osmt->log);

	OSM_LOG(&p_osmt->log, OSM_LOG_VERBOSE,
		"Getting node record for LID 0x%02X\n", cl_ntoh16(lid));

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

	record.lid = lid;

	p_context->p_osmt = p_osmt;
	user.comp_mask = IB_NR_COMPMASK_LID;
	user.attr_id = IB_MAD_ATTR_NODE_RECORD;
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
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0073: "
			"ib_query failed (%s)\n", ib_get_err_str(status));
		goto Exit;
	}

	status = p_context->result.status;

	if (status != IB_SUCCESS) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0074: "
			"ib_query failed (%s)\n", ib_get_err_str(status));
		if (status == IB_REMOTE_ERROR) {
			p_mad =
			    osm_madw_get_mad_ptr(p_context->result.
						 p_result_madw);
			OSM_LOG(&p_osmt->log, OSM_LOG_ERROR,
				"Remote error = %s\n",
				ib_get_mad_status_str(p_mad));

			status =
			    (ib_net16_t) (p_mad->status & IB_SMP_STATUS_MASK);
		}
		goto Exit;
	}

Exit:
	OSM_LOG_EXIT(&p_osmt->log);
	return (status);
}