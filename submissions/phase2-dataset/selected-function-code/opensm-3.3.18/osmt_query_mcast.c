ib_api_status_t osmt_query_mcast(IN osmtest_t * const p_osmt)
{
	ib_api_status_t status = IB_SUCCESS;
	osmv_user_query_t user;
	osmv_query_req_t req;
	osmtest_req_context_t context;
	ib_member_rec_t *p_rec;
	uint32_t i, num_recs = 0;
	cl_list_t mgids_list;
	cl_list_t *p_mgids_list;
	cl_list_iterator_t p_mgids_res;
	cl_status_t cl_status;
	cl_map_item_t *p_item, *p_next_item;
	osmtest_mgrp_t *p_mgrp;

	OSM_LOG_ENTER(&p_osmt->log);

	/*
	 * Do a blocking query for all Multicast Records in the subnet.
	 * The result is returned in the result field of the caller's
	 * context structure.
	 *
	 * The query structures are locals.
	 */

	memset(&req, 0, sizeof(req));
	memset(&user, 0, sizeof(user));

	context.p_osmt = p_osmt;
	user.attr_id = IB_MAD_ATTR_MCMEMBER_RECORD;

	req.query_type = OSMV_QUERY_USER_DEFINED;
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
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0203: "
			"ib_query failed (%s)\n", ib_get_err_str(status));
		goto Exit;
	}

	status = context.result.status;

	if (status != IB_SUCCESS) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0264: "
			"ib_query failed (%s)\n", ib_get_err_str(status));
		if (status == IB_REMOTE_ERROR)
			OSM_LOG(&p_osmt->log, OSM_LOG_ERROR,
				"Remote error = %s.\n",
				ib_get_mad_status_str(osm_madw_get_mad_ptr
						      (context.result.
						       p_result_madw)));
		goto Exit;
	}

	/* ok we have got something */
	/* First Delete the old MGID Table */
	p_next_item = cl_qmap_head(&p_osmt->exp_subn.mgrp_mlid_tbl);
	while (p_next_item != cl_qmap_end(&p_osmt->exp_subn.mgrp_mlid_tbl)) {
		p_item = p_next_item;
		p_next_item = cl_qmap_next(p_item);
		cl_qmap_remove_item(&p_osmt->exp_subn.mgrp_mlid_tbl, p_item);
		free(p_item);
	}

	cl_list_construct(&mgids_list);
	cl_list_init(&mgids_list, num_recs);
	p_mgids_list = &mgids_list;
	num_recs = context.result.result_cnt;
	OSM_LOG(&p_osmt->log, OSM_LOG_VERBOSE, "Received %u records\n",
		num_recs);

	for (i = 0; i < num_recs; i++) {
		p_rec = osmv_get_query_result(context.result.p_result_madw, i);
		p_mgids_res =
		    cl_list_find_from_head(p_mgids_list, __match_mgids,
					   &(p_rec->mgid));
		/* If returns iterator other than end of list, same mgid exists already */
		if (p_mgids_res != cl_list_end(p_mgids_list)) {
			char gid_str[INET6_ADDRSTRLEN];
			OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0265: "
				"MCG MGIDs are the same - invalid MGID : %s\n",
				inet_ntop(AF_INET6, p_rec->mgid.raw, gid_str,
					  sizeof gid_str));
			status = IB_ERROR;
			goto Exit;

		}
		osm_dump_mc_record(&p_osmt->log, p_rec, OSM_LOG_VERBOSE);
		cl_status = cl_list_insert_head(p_mgids_list, &(p_rec->mgid));
		if (cl_status) {
			OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0205: "
				"Could not add MGID to cl_list\n");
			status = IB_ERROR;
			goto Exit;
		}
		p_mgrp = (osmtest_mgrp_t *) malloc(sizeof(*p_mgrp));
		if (!p_mgrp) {
			OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0204: "
				"Could not allocate new MCG\n");
			status = IB_ERROR;
			goto Exit;
		}
		memcpy(&p_mgrp->mcmember_rec, p_rec,
		       sizeof(p_mgrp->mcmember_rec));
		cl_qmap_insert(&p_osmt->exp_subn.mgrp_mlid_tbl,
			       cl_ntoh16(p_rec->mlid), &p_mgrp->map_item);
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