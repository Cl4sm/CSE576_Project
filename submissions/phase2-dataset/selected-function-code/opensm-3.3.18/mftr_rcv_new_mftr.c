static ib_api_status_t mftr_rcv_new_mftr(IN osm_sa_t * sa,
					 IN osm_switch_t * p_sw,
					 IN cl_qlist_t * p_list,
					 IN ib_net16_t lid, IN uint16_t block,
					 IN uint8_t position)
{
	osm_sa_item_t *p_rec_item;
	ib_api_status_t status = IB_SUCCESS;
	uint16_t position_block_num;

	OSM_LOG_ENTER(sa->p_log);

	p_rec_item = malloc(SA_MFTR_RESP_SIZE);
	if (p_rec_item == NULL) {
		OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 4A02: "
			"rec_item alloc failed\n");
		status = IB_INSUFFICIENT_RESOURCES;
		goto Exit;
	}

	OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
		"New MulticastForwardingTable: sw 0x%016" PRIx64
		"\n\t\t\t\tblock %u position %u lid %u\n",
		cl_ntoh64(osm_node_get_node_guid(p_sw->p_node)),
		block, position, cl_ntoh16(lid));

	position_block_num = ((uint16_t) position << 12) |
	    (block & IB_MCAST_BLOCK_ID_MASK_HO);

	memset(p_rec_item, 0, SA_MFTR_RESP_SIZE);

	p_rec_item->resp.mft_rec.lid = lid;
	p_rec_item->resp.mft_rec.position_block_num = cl_hton16(position_block_num);

	/* copy the mft block */
	osm_switch_get_mft_block(p_sw, block, position, p_rec_item->resp.mft_rec.mft);

	cl_qlist_insert_tail(p_list, &p_rec_item->list_item);

Exit:
	OSM_LOG_EXIT(sa->p_log);
	return status;
}