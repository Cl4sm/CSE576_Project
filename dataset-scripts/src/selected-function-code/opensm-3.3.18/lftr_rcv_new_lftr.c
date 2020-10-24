static ib_api_status_t lftr_rcv_new_lftr(IN osm_sa_t * sa,
					 IN const osm_switch_t * p_sw,
					 IN cl_qlist_t * p_list,
					 IN ib_net16_t lid, IN uint16_t block)
{
	osm_sa_item_t *p_rec_item;
	ib_api_status_t status = IB_SUCCESS;

	OSM_LOG_ENTER(sa->p_log);

	p_rec_item = malloc(SA_LFTR_RESP_SIZE);
	if (p_rec_item == NULL) {
		OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 4402: "
			"rec_item alloc failed\n");
		status = IB_INSUFFICIENT_RESOURCES;
		goto Exit;
	}

	OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
		"New LinearForwardingTable: sw 0x%016" PRIx64
		"\n\t\t\t\tblock 0x%02X lid %u\n",
		cl_ntoh64(osm_node_get_node_guid(p_sw->p_node)),
		block, cl_ntoh16(lid));

	memset(p_rec_item, 0, SA_LFTR_RESP_SIZE);

	p_rec_item->resp.lft_rec.lid = lid;
	p_rec_item->resp.lft_rec.block_num = cl_hton16(block);

	/* copy the lft block */
	osm_switch_get_lft_block(p_sw, block, p_rec_item->resp.lft_rec.lft);

	cl_qlist_insert_tail(p_list, &p_rec_item->list_item);

Exit:
	OSM_LOG_EXIT(sa->p_log);
	return status;
}