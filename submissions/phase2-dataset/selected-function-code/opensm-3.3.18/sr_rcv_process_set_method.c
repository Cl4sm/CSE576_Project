static void sr_rcv_process_set_method(osm_sa_t * sa, IN osm_madw_t * p_madw)
{
	ib_sa_mad_t *p_sa_mad;
	ib_service_record_t *p_recvd_service_rec;
	ib_net64_t comp_mask;
	osm_svcr_t *p_svcr;
	osm_sa_item_t *p_sr_item;
	cl_qlist_t sr_list;

	OSM_LOG_ENTER(sa->p_log);

	CL_ASSERT(p_madw);

	p_sa_mad = osm_madw_get_sa_mad_ptr(p_madw);
	p_recvd_service_rec =
	    (ib_service_record_t *) ib_sa_mad_get_payload_ptr(p_sa_mad);

	comp_mask = p_sa_mad->comp_mask;

	if (OSM_LOG_IS_ACTIVE_V2(sa->p_log, OSM_LOG_DEBUG))
		osm_dump_service_record_v2(sa->p_log, p_recvd_service_rec,
					   FILE_ID, OSM_LOG_DEBUG);

	if ((comp_mask & (IB_SR_COMPMASK_SID | IB_SR_COMPMASK_SGID)) !=
	    (IB_SR_COMPMASK_SID | IB_SR_COMPMASK_SGID)) {
		cl_plock_release(sa->p_lock);
		OSM_LOG(sa->p_log, OSM_LOG_VERBOSE,
			"Component Mask RID check failed for METHOD_SET\n");
		osm_sa_send_error(sa, p_madw, IB_SA_MAD_STATUS_REQ_INVALID);
		goto Exit;
	}

	/* if we were not provided with a service lease make it infinite */
	if (!(comp_mask & IB_SR_COMPMASK_SLEASE)) {
		OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
			"ServiceLease Component Mask not set - using infinite lease\n");
		p_recvd_service_rec->service_lease = 0xFFFFFFFF;
	}

	/* If Record exists with matching RID */
	p_svcr = osm_svcr_get_by_rid(sa->p_subn, sa->p_log,
				     p_recvd_service_rec);

	if (p_svcr == NULL) {
		/* Create the instance of the osm_svcr_t object */
		p_svcr = osm_svcr_new(p_recvd_service_rec);
		if (p_svcr == NULL) {
			cl_plock_release(sa->p_lock);
			OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 2411: "
				"Failed to create new service record\n");

			osm_sa_send_error(sa, p_madw,
					  IB_SA_MAD_STATUS_NO_RESOURCES);
			goto Exit;
		}

		/* Add this new osm_svcr_t object to subnet object */
		osm_svcr_insert_to_db(sa->p_subn, sa->p_log, p_svcr);

	} else			/* Update the old instance of the osm_svcr_t object */
		osm_svcr_init(p_svcr, p_recvd_service_rec);

	cl_plock_release(sa->p_lock);

	if (p_recvd_service_rec->service_lease != 0xFFFFFFFF) {
#if 0
		cl_timer_trim(&sa->sr_timer,
			      p_recvd_service_rec->service_lease * 1000);
#endif
		/*  This was a bug since no check was made to see if too long */
		/*  just make sure the timer works - get a call back within a second */
		cl_timer_trim(&sa->sr_timer, 1000);
		p_svcr->modified_time = cl_get_time_stamp_sec();
	}

	p_sr_item = malloc(SA_SR_RESP_SIZE);
	if (p_sr_item == NULL) {
		OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 2412: "
			"Unable to acquire Service record\n");
		osm_sa_send_error(sa, p_madw, IB_SA_MAD_STATUS_NO_RESOURCES);
		goto Exit;
	}

	if ((comp_mask & IB_SR_COMPMASK_SPKEY) != IB_SR_COMPMASK_SPKEY)
		/* Set the Default Service P_Key in the response */
		p_recvd_service_rec->service_pkey = IB_DEFAULT_PKEY;

	p_sr_item->resp.service_rec = *p_recvd_service_rec;
	cl_qlist_init(&sr_list);

	cl_qlist_insert_tail(&sr_list, &p_sr_item->list_item);

	sr_rcv_respond(sa, p_madw, &sr_list);

Exit:
	OSM_LOG_EXIT(sa->p_log);
}