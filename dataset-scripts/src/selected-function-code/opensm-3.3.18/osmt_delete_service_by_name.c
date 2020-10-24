ib_api_status_t
osmt_delete_service_by_name(IN osmtest_t * const p_osmt,
			    IN uint8_t IsServiceExist,
			    IN char *sr_name, IN uint32_t rec_num)
{
	osmv_query_req_t req;
	osmv_user_query_t user;
	osmtest_req_context_t context;
	ib_service_record_t svc_rec;
	ib_api_status_t status;

	OSM_LOG_ENTER(&p_osmt->log);

	OSM_LOG(&p_osmt->log, OSM_LOG_INFO,
		"Trying to Delete service name: %s\n", sr_name);

	memset(&svc_rec, 0, sizeof(svc_rec));

	status = osmt_get_service_by_name(p_osmt, sr_name, rec_num, &svc_rec);
	if (status != IB_SUCCESS) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 4A15: "
			"Failed to get service: name: %s\n", sr_name);
		goto ExitNoDel;
	}

	memset(&req, 0, sizeof(req));
	memset(&context, 0, sizeof(context));
	memset(&user, 0, sizeof(user));

	/* set the new service record fields */
	memset(svc_rec.service_name, 0, sizeof(svc_rec.service_name));
	memcpy(svc_rec.service_name, sr_name,
	       (strlen(sr_name) + 1) * sizeof(char));

	/* prepare the data used for this query */
	context.p_osmt = p_osmt;
	req.timeout_ms = p_osmt->opt.transaction_timeout;
	req.query_context = &context;
	req.query_type = OSMV_QUERY_USER_DEFINED;	/*  basically a don't care here */
	req.pfn_query_cb = osmtest_query_res_cb;
	req.p_query_input = &user;
	req.flags = OSM_SA_FLAGS_SYNC;
	req.sm_key = 0;

	user.method = IB_MAD_METHOD_DELETE;
	user.attr_id = IB_MAD_ATTR_SERVICE_RECORD;
	user.comp_mask = IB_SR_COMPMASK_SNAME;
	user.p_attr = &svc_rec;

	if (p_osmt->opt.with_grh) {
		req.with_grh = 1;
		memcpy(&req.gid, &p_osmt->sm_port_gid, 16);
	}

	status = osmv_query_sa(p_osmt->h_bind, &req);
	if (status != IB_SUCCESS) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 4A16: "
			"ib_query failed (%s)\n", ib_get_err_str(status));
		goto Exit;
	}

	status = context.result.status;
	if (IsServiceExist) {
		/* If IsServiceExist = 1 then we should succeed here */
		if (status != IB_SUCCESS) {
			OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 4A17: "
				"ib_query failed (%s)\n",
				ib_get_err_str(status));

			if (status == IB_REMOTE_ERROR) {
				OSM_LOG(&p_osmt->log, OSM_LOG_ERROR,
					"ERR 4A18: Remote error = %s\n",
					ib_get_mad_status_str
					(osm_madw_get_mad_ptr
					 (context.result.p_result_madw)));
			}
		}
	} else {
		/* If IsServiceExist = 0 then we should fail here */
		if (status == IB_SUCCESS) {
			OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 4A19: "
				"Succeeded to delete service: %s which "
				"shouldn't exist", sr_name);
			status = IB_ERROR;
		} else {
			/* The deletion should have failed, since the service_name
			   shouldn't exist. */
			OSM_LOG(&p_osmt->log, OSM_LOG_ERROR,
				"IS EXPECTED ERROR ^^^^\n");
			OSM_LOG(&p_osmt->log, OSM_LOG_INFO,
				"Failed to delete service_name: %s\n", sr_name);
			status = IB_SUCCESS;
		}
	}

Exit:
	if (context.result.p_result_madw != NULL) {
		osm_mad_pool_put(&p_osmt->mad_pool,
				 context.result.p_result_madw);
		context.result.p_result_madw = NULL;
	}

ExitNoDel:
	OSM_LOG_EXIT(&p_osmt->log);
	return status;
}