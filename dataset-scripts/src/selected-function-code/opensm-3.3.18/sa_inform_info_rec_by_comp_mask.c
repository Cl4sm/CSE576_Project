static void sa_inform_info_rec_by_comp_mask(IN osm_sa_t * sa,
					    IN const osm_infr_t * p_infr,
					    osm_iir_search_ctxt_t * p_ctxt)
{
	ib_net64_t comp_mask;
	ib_net64_t portguid;
	osm_port_t *p_subscriber_port;
	osm_physp_t *p_subscriber_physp;
	const osm_physp_t *p_req_physp;
	osm_sa_item_t *p_rec_item;

	OSM_LOG_ENTER(sa->p_log);

	comp_mask = p_ctxt->comp_mask;
	p_req_physp = p_ctxt->p_req_physp;

	if (comp_mask & IB_IIR_COMPMASK_SUBSCRIBERGID &&
	    memcmp(&p_infr->inform_record.subscriber_gid,
		   &p_ctxt->subscriber_gid,
		   sizeof(p_infr->inform_record.subscriber_gid)))
		goto Exit;

	if (comp_mask & IB_IIR_COMPMASK_ENUM &&
	    p_infr->inform_record.subscriber_enum != p_ctxt->subscriber_enum)
		goto Exit;

	/* Implement any other needed search cases */

	/* Ensure pkey is shared before returning any records */
	portguid = p_infr->inform_record.subscriber_gid.unicast.interface_id;
	p_subscriber_port = osm_get_port_by_guid(sa->p_subn, portguid);
	if (p_subscriber_port == NULL) {
		OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 430D: "
			"Invalid subscriber port guid: 0x%016" PRIx64 "\n",
			cl_ntoh64(portguid));
		goto Exit;
	}

	/* get the subscriber InformInfo physical port */
	p_subscriber_physp = p_subscriber_port->p_physp;
	/* make sure that the requester and subscriber port can access each
	   other according to the current partitioning. */
	if (!osm_physp_share_pkey(sa->p_log, p_req_physp, p_subscriber_physp,
				  sa->p_subn->opt.allow_both_pkeys)) {
		OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
			"requester and subscriber ports don't share pkey\n");
		goto Exit;
	}

	p_rec_item = malloc(SA_IIR_RESP_SIZE);
	if (p_rec_item == NULL) {
		OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 430E: "
			"rec_item alloc failed\n");
		goto Exit;
	}

	memcpy(&p_rec_item->resp.inform_rec, &p_infr->inform_record,
	       sizeof(ib_inform_info_record_t));

	/*
	 * Per C15-0.2-1.16, InformInfoRecords shall always be
	 * provided with the QPN set to 0, except for the case
	 * of a trusted request, in which case the actual
	 * subscriber QPN shall be returned.
	 */
	if (p_ctxt->sm_key == 0)
		ib_inform_info_set_qpn(&p_rec_item->resp.inform_rec.inform_info, 0);

	cl_qlist_insert_tail(p_ctxt->p_list, &p_rec_item->list_item);

Exit:
	OSM_LOG_EXIT(sa->p_log);
}