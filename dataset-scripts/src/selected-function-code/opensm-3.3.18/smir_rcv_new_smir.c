static ib_api_status_t smir_rcv_new_smir(IN osm_sa_t * sa,
					 IN const osm_port_t * p_port,
					 IN cl_qlist_t * p_list,
					 IN ib_net64_t const guid,
					 IN ib_net32_t const act_count,
					 IN uint8_t const pri_state,
					 IN const osm_physp_t * p_req_physp)
{
	osm_sa_item_t *p_rec_item;
	ib_api_status_t status = IB_SUCCESS;

	OSM_LOG_ENTER(sa->p_log);

	p_rec_item = malloc(SA_SMIR_RESP_SIZE);
	if (p_rec_item == NULL) {
		OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 2801: "
			"rec_item alloc failed\n");
		status = IB_INSUFFICIENT_RESOURCES;
		goto Exit;
	}

	OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
		"New SMInfo: GUID 0x%016" PRIx64 "\n", cl_ntoh64(guid));

	memset(p_rec_item, 0, SA_SMIR_RESP_SIZE);

	p_rec_item->resp.sminfo_rec.lid = osm_port_get_base_lid(p_port);
	p_rec_item->resp.sminfo_rec.sm_info.guid = guid;
	p_rec_item->resp.sminfo_rec.sm_info.act_count = act_count;
	p_rec_item->resp.sminfo_rec.sm_info.pri_state = pri_state;

	cl_qlist_insert_tail(p_list, &p_rec_item->list_item);

Exit:
	OSM_LOG_EXIT(sa->p_log);
	return status;
}