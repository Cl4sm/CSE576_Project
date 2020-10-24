void osm_pkey_rec_rcv_process(IN void *ctx, IN void *data)
{
	osm_sa_t *sa = ctx;
	osm_madw_t *p_madw = data;
	const ib_sa_mad_t *p_rcvd_mad;
	const ib_pkey_table_record_t *p_rcvd_rec;
	const osm_port_t *p_port = NULL;
	cl_qlist_t rec_list;
	osm_pkey_search_ctxt_t context;
	ib_net64_t comp_mask;
	osm_physp_t *p_req_physp;

	CL_ASSERT(sa);

	OSM_LOG_ENTER(sa->p_log);

	CL_ASSERT(p_madw);

	p_rcvd_mad = osm_madw_get_sa_mad_ptr(p_madw);
	p_rcvd_rec =
	    (ib_pkey_table_record_t *) ib_sa_mad_get_payload_ptr(p_rcvd_mad);
	comp_mask = p_rcvd_mad->comp_mask;

	CL_ASSERT(p_rcvd_mad->attr_id == IB_MAD_ATTR_PKEY_TBL_RECORD);

	/* we only support SubnAdmGet and SubnAdmGetTable methods */
	if (p_rcvd_mad->method != IB_MAD_METHOD_GET &&
	    p_rcvd_mad->method != IB_MAD_METHOD_GETTABLE) {
		OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 4605: "
			"Unsupported Method (%s) for PKeyRecord request\n",
			ib_get_sa_method_str(p_rcvd_mad->method));
		osm_sa_send_error(sa, p_madw, IB_MAD_STATUS_UNSUP_METHOD_ATTR);
		goto Exit;
	}

	/*
	   p922 - P_KeyTableRecords shall only be provided in response
	   to trusted requests.
	   Check that the requester is a trusted one.
	 */
	if (p_rcvd_mad->sm_key != sa->p_subn->opt.sa_key) {
		/* This is not a trusted requester! */
		OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 4608: "
			"Ignoring PKeyRecord request from non-trusted requester"
			" with SM_Key 0x%016" PRIx64 "\n",
			cl_ntoh64(p_rcvd_mad->sm_key));
		osm_sa_send_error(sa, p_madw, IB_SA_MAD_STATUS_REQ_INVALID);
		goto Exit;
	}

	cl_plock_acquire(sa->p_lock);

	/* update the requester physical port */
	p_req_physp = osm_get_physp_by_mad_addr(sa->p_log, sa->p_subn,
						osm_madw_get_mad_addr_ptr
						(p_madw));
	if (p_req_physp == NULL) {
		cl_plock_release(sa->p_lock);
		OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 4604: "
			"Cannot find requester physical port\n");
		goto Exit;
	}
	OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
		"Requester port GUID 0x%" PRIx64 "\n",
		cl_ntoh64(osm_physp_get_port_guid(p_req_physp)));

	cl_qlist_init(&rec_list);

	context.p_rcvd_rec = p_rcvd_rec;
	context.p_list = &rec_list;
	context.comp_mask = p_rcvd_mad->comp_mask;
	context.sa = sa;
	context.block_num = cl_ntoh16(p_rcvd_rec->block_num);
	context.p_req_physp = p_req_physp;

	OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
		"Got Query Lid:%u(%02X), Block:0x%02X(%02X), Port:0x%02X(%02X)\n",
		cl_ntoh16(p_rcvd_rec->lid),
		(comp_mask & IB_PKEY_COMPMASK_LID) != 0, p_rcvd_rec->port_num,
		(comp_mask & IB_PKEY_COMPMASK_PORT) != 0, context.block_num,
		(comp_mask & IB_PKEY_COMPMASK_BLOCK) != 0);

	/*
	   If the user specified a LID, it obviously narrows our
	   work load, since we don't have to search every port
	 */
	if (comp_mask & IB_PKEY_COMPMASK_LID) {
		p_port = osm_get_port_by_lid(sa->p_subn, p_rcvd_rec->lid);
		if (!p_port)
			OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 460B: "
				"No port found with LID %u\n",
				cl_ntoh16(p_rcvd_rec->lid));
		else
			sa_pkey_by_comp_mask(sa, p_port, &context);
	} else
		cl_qmap_apply_func(&sa->p_subn->port_guid_tbl,
				   sa_pkey_by_comp_mask_cb, &context);

	cl_plock_release(sa->p_lock);

	osm_sa_respond(sa, p_madw, sizeof(ib_pkey_table_record_t), &rec_list);

Exit:
	OSM_LOG_EXIT(sa->p_log);
}