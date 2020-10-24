static void sr_rcv_process_delete_method(osm_sa_t * sa, IN osm_madw_t * p_madw)
{
	ib_sa_mad_t *p_sa_mad;
	ib_service_record_t *p_recvd_service_rec;
	osm_svcr_t *p_svcr;
	osm_sa_item_t *p_sr_item;
	cl_qlist_t sr_list;

	OSM_LOG_ENTER(sa->p_log);

	CL_ASSERT(p_madw);

	p_sa_mad = osm_madw_get_sa_mad_ptr(p_madw);
	p_recvd_service_rec =
	    (ib_service_record_t *) ib_sa_mad_get_payload_ptr(p_sa_mad);

	if (OSM_LOG_IS_ACTIVE_V2(sa->p_log, OSM_LOG_DEBUG))
		osm_dump_service_record_v2(sa->p_log, p_recvd_service_rec,
					   FILE_ID, OSM_LOG_DEBUG);

	/* If Record exists with matching RID */
	p_svcr = osm_svcr_get_by_rid(sa->p_subn, sa->p_log,
				     p_recvd_service_rec);

	if (p_svcr == NULL) {
		cl_plock_release(sa->p_lock);
		OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
			"No records matched the RID\n");
		osm_sa_send_error(sa, p_madw, IB_SA_MAD_STATUS_NO_RECORDS);
		goto Exit;
	}

	osm_svcr_remove_from_db(sa->p_subn, sa->p_log, p_svcr);
	cl_plock_release(sa->p_lock);

	p_sr_item = malloc(SA_SR_RESP_SIZE);
	if (p_sr_item == NULL) {
		OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 2413: "
			"Unable to acquire Service record\n");
		osm_sa_send_error(sa, p_madw, IB_SA_MAD_STATUS_NO_RESOURCES);
		osm_svcr_delete(p_svcr);
		goto Exit;
	}

	/* provide back the copy of the record */
	p_sr_item->resp.service_rec = p_svcr->service_record;
	cl_qlist_init(&sr_list);

	cl_qlist_insert_tail(&sr_list, &p_sr_item->list_item);

	osm_svcr_delete(p_svcr);

	sr_rcv_respond(sa, p_madw, &sr_list);

Exit:
	OSM_LOG_EXIT(sa->p_log);
	return;
}