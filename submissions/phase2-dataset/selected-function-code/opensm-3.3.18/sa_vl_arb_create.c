static void sa_vl_arb_create(IN osm_sa_t * sa, IN osm_physp_t * p_physp,
			     IN osm_vl_arb_search_ctxt_t * p_ctxt,
			     IN uint8_t block)
{
	osm_sa_item_t *p_rec_item;
	uint16_t lid;

	OSM_LOG_ENTER(sa->p_log);

	p_rec_item = malloc(SA_VLA_RESP_SIZE);
	if (p_rec_item == NULL) {
		OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 2A02: "
			"rec_item alloc failed\n");
		goto Exit;
	}

	if (p_physp->p_node->node_info.node_type != IB_NODE_TYPE_SWITCH)
		lid = p_physp->port_info.base_lid;
	else
		lid = osm_node_get_base_lid(p_physp->p_node, 0);

	OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
		"New VLArbitration for: port 0x%016" PRIx64
		", lid %u, port %u Block:%u\n",
		cl_ntoh64(osm_physp_get_port_guid(p_physp)),
		cl_ntoh16(lid), osm_physp_get_port_num(p_physp), block);

	memset(p_rec_item, 0, SA_VLA_RESP_SIZE);

	p_rec_item->resp.vlarb_rec.lid = lid;
	p_rec_item->resp.vlarb_rec.port_num = osm_physp_get_port_num(p_physp);
	p_rec_item->resp.vlarb_rec.block_num = block;
	p_rec_item->resp.vlarb_rec.vl_arb_tbl = *(osm_physp_get_vla_tbl(p_physp, block));

	cl_qlist_insert_tail(p_ctxt->p_list, &p_rec_item->list_item);

Exit:
	OSM_LOG_EXIT(sa->p_log);
}