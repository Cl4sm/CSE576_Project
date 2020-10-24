ib_api_status_t
osmt_register_service(IN osmtest_t * const p_osmt,
		      IN ib_net64_t service_id,
		      IN ib_net16_t service_pkey,
		      IN ib_net32_t service_lease,
		      IN uint8_t service_key_lsb, IN char *service_name)
{
	osmv_query_req_t req;
	osmv_user_query_t user;
	osmtest_req_context_t context;
	ib_service_record_t svc_rec;
	osm_log_t *p_log = &p_osmt->log;
	ib_api_status_t status;

	OSM_LOG_ENTER(p_log);

	OSM_LOG(&p_osmt->log, OSM_LOG_INFO,
		"Registering service: name: %s id: 0x%" PRIx64 "\n",
		service_name, cl_ntoh64(service_id));

	memset(&req, 0, sizeof(req));
	memset(&context, 0, sizeof(context));
	memset(&user, 0, sizeof(user));
	memset(&svc_rec, 0, sizeof(svc_rec));

	/* set the new service record fields */
	svc_rec.service_id = service_id;
	svc_rec.service_pkey = service_pkey;
	svc_rec.service_gid.unicast.prefix = p_osmt->local_port_gid.unicast.prefix;
	svc_rec.service_gid.unicast.interface_id = p_osmt->local_port.port_guid;
	svc_rec.service_lease = service_lease;
	memset(&svc_rec.service_key, 0, 16 * sizeof(uint8_t));
	svc_rec.service_key[0] = service_key_lsb;
	memset(svc_rec.service_name, 0, sizeof(svc_rec.service_name));
	memcpy(svc_rec.service_name, service_name,
	       (strlen(service_name) + 1) * sizeof(char));

	/* prepare the data used for this query */
	/*  sa_mad_data.method = IB_MAD_METHOD_SET; */
	/*  sa_mad_data.sm_key = 0; */

	context.p_osmt = p_osmt;
	req.query_context = &context;
	req.query_type = OSMV_QUERY_USER_DEFINED;
	req.pfn_query_cb = osmtest_query_res_cb;
	req.p_query_input = &user;
	req.flags = OSM_SA_FLAGS_SYNC;
	req.sm_key = 0;
	req.timeout_ms = p_osmt->opt.transaction_timeout;

	user.method = IB_MAD_METHOD_SET;
	user.attr_id = IB_MAD_ATTR_SERVICE_RECORD;
	if (ib_pkey_is_invalid(service_pkey)) {
		/* if given an invalid service_pkey - don't turn the PKEY compmask on */
		user.comp_mask = IB_SR_COMPMASK_SID |
		    IB_SR_COMPMASK_SGID |
		    IB_SR_COMPMASK_SLEASE |
		    IB_SR_COMPMASK_SKEY | IB_SR_COMPMASK_SNAME;
	} else {
		user.comp_mask = IB_SR_COMPMASK_SID |
		    IB_SR_COMPMASK_SGID |
		    IB_SR_COMPMASK_SPKEY |
		    IB_SR_COMPMASK_SLEASE |
		    IB_SR_COMPMASK_SKEY | IB_SR_COMPMASK_SNAME;
	}
	user.p_attr = &svc_rec;

	if (p_osmt->opt.with_grh) {
		req.with_grh = 1;
		memcpy(&req.gid, &p_osmt->sm_port_gid, 16);
	}

	status = osmv_query_sa(p_osmt->h_bind, &req);
	if (status != IB_SUCCESS) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 4A01: "
			"ib_query failed (%s)\n", ib_get_err_str(status));
		goto Exit;
	}

	status = context.result.status;

	if (status != IB_SUCCESS) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 4A02: "
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

Exit:
	if (context.result.p_result_madw != NULL) {
		osm_mad_pool_put(&p_osmt->mad_pool,
				 context.result.p_result_madw);
		context.result.p_result_madw = NULL;
	}

	OSM_LOG_EXIT(&p_osmt->log);
	return status;
}