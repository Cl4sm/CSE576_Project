void osm_lr_rcv_process(IN void *context, IN void *data)
{
	osm_sa_t *sa = context;
	osm_madw_t *p_madw = data;
	const ib_link_record_t *p_lr;
	const ib_sa_mad_t *p_sa_mad;
	const osm_port_t *p_src_port;
	const osm_port_t *p_dest_port;
	cl_qlist_t lr_list;
	ib_net16_t status;
	osm_physp_t *p_req_physp;

	OSM_LOG_ENTER(sa->p_log);

	CL_ASSERT(p_madw);

	p_sa_mad = osm_madw_get_sa_mad_ptr(p_madw);
	p_lr = ib_sa_mad_get_payload_ptr(p_sa_mad);

	CL_ASSERT(p_sa_mad->attr_id == IB_MAD_ATTR_LINK_RECORD);

	/* we only support SubnAdmGet and SubnAdmGetTable methods */
	if (p_sa_mad->method != IB_MAD_METHOD_GET &&
	    p_sa_mad->method != IB_MAD_METHOD_GETTABLE) {
		OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 1804: "
			"Unsupported Method (%s) for LinkRecord request\n",
			ib_get_sa_method_str(p_sa_mad->method));
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
		OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 1805: "
			"Cannot find requester physical port\n");
		goto Exit;
	}

	if (OSM_LOG_IS_ACTIVE_V2(sa->p_log, OSM_LOG_DEBUG)) {
		OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
			"Requester port GUID 0x%" PRIx64 "\n",
			cl_ntoh64(osm_physp_get_port_guid(p_req_physp)));
		osm_dump_link_record_v2(sa->p_log, p_lr, FILE_ID, OSM_LOG_DEBUG);
	}

	cl_qlist_init(&lr_list);

	/*
	   Most SA functions (including this one) are read-only on the
	   subnet object, so we grab the lock non-exclusively.
	 */
	status = lr_rcv_get_end_points(sa, p_madw, &p_src_port, &p_dest_port);

	if (status == IB_SA_MAD_STATUS_SUCCESS)
		lr_rcv_get_port_links(sa, p_lr, p_src_port, p_dest_port,
				      p_sa_mad->comp_mask, &lr_list,
				      p_req_physp);

	cl_plock_release(sa->p_lock);

	osm_sa_respond(sa, p_madw, sizeof(ib_link_record_t), &lr_list);

Exit:
	OSM_LOG_EXIT(sa->p_log);
}