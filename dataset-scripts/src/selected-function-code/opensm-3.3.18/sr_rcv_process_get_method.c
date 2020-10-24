static void sr_rcv_process_get_method(osm_sa_t * sa, IN osm_madw_t * p_madw)
{
	ib_sa_mad_t *p_sa_mad;
	ib_service_record_t *p_recvd_service_rec;
	osm_sr_match_item_t sr_match_item;
	osm_sr_search_ctxt_t context;
	osm_physp_t *p_req_physp;

	OSM_LOG_ENTER(sa->p_log);

	CL_ASSERT(p_madw);

	/* Grab the lock */
	cl_plock_acquire(sa->p_lock);

	/* update the requester physical port */
	p_req_physp = osm_get_physp_by_mad_addr(sa->p_log, sa->p_subn,
						osm_madw_get_mad_addr_ptr
						(p_madw));
	if (p_req_physp == NULL) {
		cl_plock_release(sa->p_lock);
		OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 2409: "
			"Cannot find requester physical port\n");
		goto Exit;
	}

	p_sa_mad = osm_madw_get_sa_mad_ptr(p_madw);
	p_recvd_service_rec =
	    (ib_service_record_t *) ib_sa_mad_get_payload_ptr(p_sa_mad);

	if (OSM_LOG_IS_ACTIVE_V2(sa->p_log, OSM_LOG_DEBUG)) {
		OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
			"Requester port GUID 0x%" PRIx64 "\n",
			cl_ntoh64(osm_physp_get_port_guid(p_req_physp)));
		osm_dump_service_record_v2(sa->p_log, p_recvd_service_rec,
					   FILE_ID, OSM_LOG_DEBUG);
	}

	cl_qlist_init(&sr_match_item.sr_list);
	sr_match_item.p_service_rec = p_recvd_service_rec;
	sr_match_item.comp_mask = p_sa_mad->comp_mask;
	sr_match_item.sa = sa;

	context.p_sr_item = &sr_match_item;
	context.p_req_physp = p_req_physp;

	cl_qlist_apply_func(&sa->p_subn->sa_sr_list, get_matching_sr, &context);

	cl_plock_release(sa->p_lock);

	if (p_sa_mad->method == IB_MAD_METHOD_GET &&
	    cl_qlist_count(&sr_match_item.sr_list) == 0) {
		OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
			"No records matched the Service Record query\n");
		osm_sa_send_error(sa, p_madw, IB_SA_MAD_STATUS_NO_RECORDS);
		goto Exit;
	}

	sr_rcv_respond(sa, p_madw, &sr_match_item.sr_list);

Exit:
	OSM_LOG_EXIT(sa->p_log);
	return;
}