static void sa_pkey_create(IN osm_sa_t * sa, IN osm_physp_t * p_physp,
			   IN osm_pkey_search_ctxt_t * p_ctxt,
			   IN uint16_t block)
{
	osm_sa_item_t *p_rec_item;
	uint16_t lid;
	ib_pkey_table_t *tbl;

	OSM_LOG_ENTER(sa->p_log);

	p_rec_item = malloc(SA_PKEY_RESP_SIZE);
	if (p_rec_item == NULL) {
		OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 4602: "
			"rec_item alloc failed\n");
		goto Exit;
	}

	if (p_physp->p_node->node_info.node_type != IB_NODE_TYPE_SWITCH)
		lid = p_physp->port_info.base_lid;
	else
		lid = osm_node_get_base_lid(p_physp->p_node, 0);

	OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
		"New P_Key table for: port 0x%016" PRIx64
		", lid %u, port %u Block:%u\n",
		cl_ntoh64(osm_physp_get_port_guid(p_physp)),
		cl_ntoh16(lid), osm_physp_get_port_num(p_physp), block);

	memset(p_rec_item, 0, SA_PKEY_RESP_SIZE);

	p_rec_item->resp.pkey_rec.lid = lid;
	p_rec_item->resp.pkey_rec.block_num = block;
	p_rec_item->resp.pkey_rec.port_num = osm_physp_get_port_num(p_physp);
	/* FIXME: There are ninf.PartitionCap or swinf.PartitionEnforcementCap
	   pkey entries so everything in that range is a valid block number
	   even if opensm is not using it. Return 0. However things outside
	   that range should return no entries. Not sure how to figure that
	   here? The range of pkey_tbl can be less than the cap, so
	   this falsely triggers. */
	tbl = osm_pkey_tbl_block_get(osm_physp_get_pkey_tbl(p_physp), block);
	if (tbl)
		p_rec_item->resp.pkey_rec.pkey_tbl = *tbl;

	cl_qlist_insert_tail(p_ctxt->p_list, &p_rec_item->list_item);

Exit:
	OSM_LOG_EXIT(sa->p_log);
}