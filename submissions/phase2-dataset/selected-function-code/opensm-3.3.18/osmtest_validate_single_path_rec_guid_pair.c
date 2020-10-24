static ib_api_status_t
osmtest_validate_single_path_rec_guid_pair(IN osmtest_t * const p_osmt,
					   IN const osmv_guid_pair_t *
					   const p_pair)
{
	osmtest_req_context_t context;
	const ib_path_rec_t *p_rec;
	cl_status_t status = IB_SUCCESS;
	size_t num_recs;
	osmv_query_req_t req;
	uint32_t i;
	boolean_t got_error = FALSE;

	OSM_LOG_ENTER(&p_osmt->log);

	memset(&req, 0, sizeof(req));
	memset(&context, 0, sizeof(context));

	OSM_LOG(&p_osmt->log, OSM_LOG_ERROR,
		"\n\t\t\t\tChecking src 0x%016" PRIx64
		" to dest 0x%016" PRIx64 "\n",
		cl_ntoh64(p_pair->src_guid), cl_ntoh64(p_pair->dest_guid));

	context.p_osmt = p_osmt;

	req.timeout_ms = p_osmt->opt.transaction_timeout;
	req.retry_cnt = p_osmt->opt.retry_count;
	req.flags = OSM_SA_FLAGS_SYNC;
	req.query_context = &context;
	req.pfn_query_cb = osmtest_query_res_cb;

	req.query_type = OSMV_QUERY_PATH_REC_BY_PORT_GUIDS;
	req.p_query_input = p_pair;
	req.sm_key = 0;

	if (p_osmt->opt.with_grh) {
		req.with_grh = 1;
		memcpy(&req.gid, &p_osmt->sm_port_gid, 16);
	}

	status = osmv_query_sa(p_osmt->h_bind, &req);
	if (status != IB_SUCCESS) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0110: "
			"ib_query failed (%s)\n", ib_get_err_str(status));
		goto Exit;
	}

	status = context.result.status;

	if (status != IB_SUCCESS) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0111: "
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
	OSM_LOG(&p_osmt->log, OSM_LOG_VERBOSE, "%zu records\n", num_recs);

	for (i = 0; i < num_recs; i++) {
		p_rec =
		    osmv_get_query_path_rec(context.result.p_result_madw, i);

		/*
		 * Make sure the GUID values are correct
		 */
		if (p_rec->dgid.unicast.interface_id != p_pair->dest_guid) {
			OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0112: "
				"Destination GUID mismatch\n"
				"\t\t\t\texpected 0x%016" PRIx64
				", received 0x%016" PRIx64 "\n",
				cl_ntoh64(p_pair->dest_guid),
				cl_ntoh64(p_rec->dgid.unicast.interface_id));
			got_error = TRUE;
		}

		if (p_rec->sgid.unicast.interface_id != p_pair->src_guid) {
			OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0113: "
				"Source GUID mismatch\n"
				"\t\t\t\texpected 0x%016" PRIx64
				", received 0x%016" PRIx64 ".\n",
				cl_ntoh64(p_pair->src_guid),
				cl_ntoh64(p_rec->sgid.unicast.interface_id));
			got_error = TRUE;
		}

		status = osmtest_validate_path_rec(p_osmt, p_rec);
		if (status != IB_SUCCESS) {
			OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0114: "
				"osmtest_validate_path_rec failed (%s)\n",
				ib_get_err_str(status));
			got_error = TRUE;
		}
		if (got_error || (status != IB_SUCCESS)) {
			osm_dump_path_record(&p_osmt->log, p_rec,
					     OSM_LOG_VERBOSE);
			if (status == IB_SUCCESS)
				status = IB_ERROR;
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