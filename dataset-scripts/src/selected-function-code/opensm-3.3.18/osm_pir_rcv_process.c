void osm_pir_rcv_process(IN void *ctx, IN void *data)
{
	osm_sa_t *sa = ctx;
	osm_madw_t *p_madw = data;
	const ib_sa_mad_t *p_rcvd_mad;
	const ib_portinfo_record_t *p_rcvd_rec;
	const osm_port_t *p_port = NULL;
	cl_qlist_t rec_list;
	osm_pir_search_ctxt_t context;
	ib_net64_t comp_mask;
	osm_physp_t *p_req_physp;

	CL_ASSERT(sa);

	OSM_LOG_ENTER(sa->p_log);

	CL_ASSERT(p_madw);

	p_rcvd_mad = osm_madw_get_sa_mad_ptr(p_madw);
	p_rcvd_rec =
	    (ib_portinfo_record_t *) ib_sa_mad_get_payload_ptr(p_rcvd_mad);
	comp_mask = p_rcvd_mad->comp_mask;

	CL_ASSERT(p_rcvd_mad->attr_id == IB_MAD_ATTR_PORTINFO_RECORD);

	/* we only support SubnAdmGet and SubnAdmGetTable methods */
	if (p_rcvd_mad->method != IB_MAD_METHOD_GET &&
	    p_rcvd_mad->method != IB_MAD_METHOD_GETTABLE) {
		OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 2105: "
			"Unsupported Method (%s) for PortInfoRecord request\n",
			ib_get_sa_method_str(p_rcvd_mad->method));
		osm_sa_send_error(sa, p_madw, IB_MAD_STATUS_UNSUP_METHOD_ATTR);
		goto Exit;
	}

	cl_plock_acquire(sa->p_lock);

	/* update the requester physical port */
	p_req_physp = osm_get_physp_by_mad_addr(sa->p_log, sa->p_subn,
						osm_madw_get_mad_addr_ptr
						(p_madw));
	if (p_req_physp == NULL) {
		cl_plock_release(sa->p_lock);
		OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 2104: "
			"Cannot find requester physical port\n");
		goto Exit;
	}

	if (OSM_LOG_IS_ACTIVE_V2(sa->p_log, OSM_LOG_DEBUG)) {
		OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
			"Requester port GUID 0x%" PRIx64 "\n",
			cl_ntoh64(osm_physp_get_port_guid(p_req_physp)));
		osm_dump_portinfo_record_v2(sa->p_log, p_rcvd_rec, FILE_ID, OSM_LOG_DEBUG);
	}

	cl_qlist_init(&rec_list);

	context.p_rcvd_rec = p_rcvd_rec;
	context.p_list = &rec_list;
	context.comp_mask = p_rcvd_mad->comp_mask;
	context.sa = sa;
	context.p_req_physp = p_req_physp;
	context.is_enhanced_comp_mask =
	    cl_ntoh32(p_rcvd_mad->attr_mod) & (1 << 31);

	/*
	   If the user specified a LID, it obviously narrows our
	   work load, since we don't have to search every port
	 */
	if (comp_mask & (IB_PIR_COMPMASK_LID | IB_PIR_COMPMASK_BASELID)) {
		p_port = osm_get_port_by_lid(sa->p_subn, p_rcvd_rec->lid);
		if (p_port)
			sa_pir_by_comp_mask(sa, p_port->p_node, &context);
		else
			OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 2109: "
				"No port found with requested LID %u\n",
				cl_ntoh16(p_rcvd_rec->lid));
	} else
		cl_qmap_apply_func(&sa->p_subn->node_guid_tbl,
				   sa_pir_by_comp_mask_cb, &context);

	cl_plock_release(sa->p_lock);

	/*
	   p922 - The M_Key returned shall be zero, except in the case of a
	   trusted request.
	   Note: In the mad controller we check that the SM_Key received on
	   the mad is valid. Meaning - is either zero or equal to the local
	   sm_key.
	 */
	if (!p_rcvd_mad->sm_key) {
		osm_sa_item_t *item;
		for (item = (osm_sa_item_t *) cl_qlist_head(&rec_list);
		     item != (osm_sa_item_t *) cl_qlist_end(&rec_list);
		     item = (osm_sa_item_t *) cl_qlist_next(&item->list_item))
			item->resp.port_rec.port_info.m_key = 0;
	}

	osm_sa_respond(sa, p_madw, sizeof(ib_portinfo_record_t), &rec_list);

Exit:
	OSM_LOG_EXIT(sa->p_log);
}