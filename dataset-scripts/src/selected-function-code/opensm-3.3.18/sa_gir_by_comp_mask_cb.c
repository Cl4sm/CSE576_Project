static void sa_gir_by_comp_mask_cb(IN cl_map_item_t * p_map_item, IN void *cxt)
{
	const osm_gir_search_ctxt_t *p_ctxt = cxt;
	osm_node_t *const p_node = (osm_node_t *) p_map_item;
	const ib_guidinfo_record_t *const p_rcvd_rec = p_ctxt->p_rcvd_rec;
	const osm_physp_t *const p_req_physp = p_ctxt->p_req_physp;
	osm_sa_t *sa = p_ctxt->sa;
	const ib_guid_info_t *p_comp_gi;
	ib_net64_t const comp_mask = p_ctxt->comp_mask;
	ib_net64_t match_port_guid = 0;
	ib_net16_t match_lid = 0;
	uint8_t match_block_num = 255;

	OSM_LOG_ENTER(p_ctxt->sa->p_log);

	if (comp_mask & IB_GIR_COMPMASK_LID)
		match_lid = p_rcvd_rec->lid;

	if (comp_mask & IB_GIR_COMPMASK_BLOCKNUM)
		match_block_num = p_rcvd_rec->block_num;

	p_comp_gi = &p_rcvd_rec->guid_info;
	/* Different rule for block 0 v. other blocks */
	if (comp_mask & IB_GIR_COMPMASK_GID0) {
		if (!p_rcvd_rec->block_num)
			match_port_guid = osm_physp_get_port_guid(p_req_physp);
		if (p_comp_gi->guid[0] != match_port_guid)
			goto Exit;
	}

	if (comp_mask & IB_GIR_COMPMASK_GID1) {
		if (p_comp_gi->guid[1] != 0)
			goto Exit;
	}

	if (comp_mask & IB_GIR_COMPMASK_GID2) {
		if (p_comp_gi->guid[2] != 0)
			goto Exit;
	}

	if (comp_mask & IB_GIR_COMPMASK_GID3) {
		if (p_comp_gi->guid[3] != 0)
			goto Exit;
	}

	if (comp_mask & IB_GIR_COMPMASK_GID4) {
		if (p_comp_gi->guid[4] != 0)
			goto Exit;
	}

	if (comp_mask & IB_GIR_COMPMASK_GID5) {
		if (p_comp_gi->guid[5] != 0)
			goto Exit;
	}

	if (comp_mask & IB_GIR_COMPMASK_GID6) {
		if (p_comp_gi->guid[6] != 0)
			goto Exit;
	}

	if (comp_mask & IB_GIR_COMPMASK_GID7) {
		if (p_comp_gi->guid[7] != 0)
			goto Exit;
	}

	sa_gir_create_gir(sa, p_node, p_ctxt->p_list, match_port_guid,
			  match_lid, p_req_physp, match_block_num);

Exit:
	OSM_LOG_EXIT(p_ctxt->sa->p_log);
}