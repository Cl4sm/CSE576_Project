static ib_api_status_t gir_rcv_new_gir(IN osm_sa_t * sa,
				       IN const osm_node_t * p_node,
				       IN cl_qlist_t * p_list,
				       IN ib_net64_t const match_port_guid,
				       IN ib_net16_t const match_lid,
				       IN const osm_physp_t * p_physp,
				       IN uint8_t const block_num)
{
	osm_sa_item_t *p_rec_item;
	ib_api_status_t status = IB_SUCCESS;

	OSM_LOG_ENTER(sa->p_log);

	p_rec_item = malloc(SA_GIR_RESP_SIZE);
	if (p_rec_item == NULL) {
		OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 5102: "
			"rec_item alloc failed\n");
		status = IB_INSUFFICIENT_RESOURCES;
		goto Exit;
	}

	OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
		"New GUIDInfoRecord: lid %u, block num %d\n",
		cl_ntoh16(match_lid), block_num);

	memset(p_rec_item, 0, SA_GIR_RESP_SIZE);

	p_rec_item->resp.guid_rec.lid = match_lid;
	p_rec_item->resp.guid_rec.block_num = block_num;
	if (p_physp->p_guids)
		memcpy(&p_rec_item->resp.guid_rec.guid_info,
		       *p_physp->p_guids + block_num * GUID_TABLE_MAX_ENTRIES,
		       sizeof(ib_guid_info_t));
	else if (!block_num)
		p_rec_item->resp.guid_rec.guid_info.guid[0] = osm_physp_get_port_guid(p_physp);

	cl_qlist_insert_tail(p_list, &p_rec_item->list_item);

Exit:
	OSM_LOG_EXIT(sa->p_log);
	return status;
}