static void sa_slvl_create(IN osm_sa_t * sa, IN const osm_physp_t * p_physp,
			   IN osm_slvl_search_ctxt_t * p_ctxt,
			   IN uint8_t in_port_idx)
{
	osm_sa_item_t *p_rec_item;
	uint16_t lid;

	OSM_LOG_ENTER(sa->p_log);

	p_rec_item = malloc(SA_SLVL_RESP_SIZE);
	if (p_rec_item == NULL) {
		OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 2602: "
			"rec_item alloc failed\n");
		goto Exit;
	}

	if (p_physp->p_node->node_info.node_type != IB_NODE_TYPE_SWITCH)
		lid = p_physp->port_info.base_lid;
	else
		lid = osm_node_get_base_lid(p_physp->p_node, 0);

	OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
		"New SLtoVL Map for: OUT port 0x%016" PRIx64
		", lid 0x%X, port %u to In Port:%u\n",
		cl_ntoh64(osm_physp_get_port_guid(p_physp)),
		cl_ntoh16(lid), osm_physp_get_port_num(p_physp), in_port_idx);

	memset(p_rec_item, 0, SA_SLVL_RESP_SIZE);

	p_rec_item->resp.slvl_rec.lid = lid;
	if (p_physp->p_node->node_info.node_type == IB_NODE_TYPE_SWITCH) {
		p_rec_item->resp.slvl_rec.out_port_num = osm_physp_get_port_num(p_physp);
		p_rec_item->resp.slvl_rec.in_port_num = in_port_idx;
	}
	p_rec_item->resp.slvl_rec.slvl_tbl =
	    *(osm_physp_get_slvl_tbl(p_physp, in_port_idx));

	cl_qlist_insert_tail(p_ctxt->p_list, &p_rec_item->list_item);

Exit:
	OSM_LOG_EXIT(sa->p_log);
}