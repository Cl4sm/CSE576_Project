ib_api_status_t osm_resp_send(IN osm_sm_t * sm,
			      IN const osm_madw_t * p_req_madw,
			      IN ib_net16_t mad_status,
			      IN const uint8_t * p_payload)
{
	const ib_smp_t *p_req_smp;
	ib_smp_t *p_smp;
	osm_madw_t *p_madw;
	ib_api_status_t status = IB_SUCCESS;

	OSM_LOG_ENTER(sm->p_log);

	CL_ASSERT(p_req_madw);
	CL_ASSERT(p_payload);

	/* do nothing if we are exiting ... */
	if (osm_exit_flag)
		goto Exit;

	p_madw = osm_mad_pool_get(sm->p_mad_pool,
				  osm_madw_get_bind_handle(p_req_madw),
				  MAD_BLOCK_SIZE, NULL);

	if (p_madw == NULL) {
		OSM_LOG(sm->p_log, OSM_LOG_ERROR,
			"ERR 1301: Unable to acquire MAD\n");
		status = IB_INSUFFICIENT_RESOURCES;
		goto Exit;
	}

	/*
	   Copy the request smp to the response smp, then just
	   update the necessary fields.
	 */
	p_smp = osm_madw_get_smp_ptr(p_madw);
	p_req_smp = osm_madw_get_smp_ptr(p_req_madw);
	resp_make_resp_smp(sm, p_req_smp, mad_status, p_payload, p_smp);
	p_madw->mad_addr.dest_lid =
	    p_req_madw->mad_addr.addr_type.smi.source_lid;
	p_madw->mad_addr.addr_type.smi.source_lid =
	    p_req_madw->mad_addr.dest_lid;

	p_madw->resp_expected = FALSE;
	p_madw->fail_msg = CL_DISP_MSGID_NONE;

	OSM_LOG(sm->p_log, OSM_LOG_DEBUG, "Responding to %s (0x%X)"
		"\n\t\t\t\tattribute modifier 0x%X, TID 0x%" PRIx64 "\n",
		ib_get_sm_attr_str(p_smp->attr_id), cl_ntoh16(p_smp->attr_id),
		cl_ntoh32(p_smp->attr_mod), cl_ntoh64(p_smp->trans_id));

	osm_vl15_post(sm->p_vl15, p_madw);

Exit:
	OSM_LOG_EXIT(sm->p_log);
	return status;
}