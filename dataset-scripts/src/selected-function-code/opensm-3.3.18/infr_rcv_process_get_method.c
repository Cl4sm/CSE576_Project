static void infr_rcv_process_get_method(osm_sa_t * sa, IN osm_madw_t * p_madw)
{
	char gid_str[INET6_ADDRSTRLEN];
	ib_sa_mad_t *p_rcvd_mad;
	const ib_inform_info_record_t *p_rcvd_rec;
	cl_qlist_t rec_list;
	osm_iir_search_ctxt_t context;
	osm_physp_t *p_req_physp;
	osm_sa_item_t *item;

	OSM_LOG_ENTER(sa->p_log);

	CL_ASSERT(p_madw);
	p_rcvd_mad = osm_madw_get_sa_mad_ptr(p_madw);
	p_rcvd_rec =
	    (ib_inform_info_record_t *) ib_sa_mad_get_payload_ptr(p_rcvd_mad);

	cl_plock_acquire(sa->p_lock);

	/* update the requester physical port */
	p_req_physp = osm_get_physp_by_mad_addr(sa->p_log, sa->p_subn,
						osm_madw_get_mad_addr_ptr
						(p_madw));
	if (p_req_physp == NULL) {
		cl_plock_release(sa->p_lock);
		OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 4309: "
			"Cannot find requester physical port\n");
		goto Exit;
	}

	if (OSM_LOG_IS_ACTIVE_V2(sa->p_log, OSM_LOG_DEBUG)) {
		OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
			"Requester port GUID 0x%" PRIx64 "\n",
			cl_ntoh64(osm_physp_get_port_guid(p_req_physp)));
		osm_dump_inform_info_record_v2(sa->p_log, p_rcvd_rec,
					       FILE_ID, OSM_LOG_DEBUG);
	}

	cl_qlist_init(&rec_list);

	context.p_rcvd_rec = p_rcvd_rec;
	context.p_list = &rec_list;
	context.comp_mask = p_rcvd_mad->comp_mask;
	context.subscriber_gid = p_rcvd_rec->subscriber_gid;
	context.subscriber_enum = p_rcvd_rec->subscriber_enum;
	context.sa = sa;
	context.p_req_physp = p_req_physp;
	context.sm_key = p_rcvd_mad->sm_key;

	OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
		"Query Subscriber GID:%s(%02X) Enum:0x%X(%02X)\n",
		inet_ntop(AF_INET6, p_rcvd_rec->subscriber_gid.raw,
			  gid_str, sizeof gid_str),
		(p_rcvd_mad->comp_mask & IB_IIR_COMPMASK_SUBSCRIBERGID) != 0,
		cl_ntoh16(p_rcvd_rec->subscriber_enum),
		(p_rcvd_mad->comp_mask & IB_IIR_COMPMASK_ENUM) != 0);

	cl_qlist_apply_func(&sa->p_subn->sa_infr_list,
			    sa_inform_info_rec_by_comp_mask_cb, &context);

	/* clear reserved and pad fields in InformInfoRecord */
	for (item = (osm_sa_item_t *) cl_qlist_head(&rec_list);
	     item != (osm_sa_item_t *) cl_qlist_end(&rec_list);
	     item = (osm_sa_item_t *) cl_qlist_next(&item->list_item)) {
		memset(item->resp.inform_rec.reserved, 0, sizeof(item->resp.inform_rec.reserved));
		memset(item->resp.inform_rec.pad, 0, sizeof(item->resp.inform_rec.pad));
	}

	cl_plock_release(sa->p_lock);

	osm_sa_respond(sa, p_madw, sizeof(ib_inform_info_record_t), &rec_list);

Exit:
	OSM_LOG_EXIT(sa->p_log);
}