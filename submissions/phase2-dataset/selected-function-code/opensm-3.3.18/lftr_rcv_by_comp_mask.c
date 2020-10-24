static void lftr_rcv_by_comp_mask(IN cl_map_item_t * p_map_item, IN void *cxt)
{
	const osm_lftr_search_ctxt_t *p_ctxt = cxt;
	const osm_switch_t *p_sw = (osm_switch_t *) p_map_item;
	const ib_lft_record_t *const p_rcvd_rec = p_ctxt->p_rcvd_rec;
	osm_sa_t *sa = p_ctxt->sa;
	ib_net64_t const comp_mask = p_ctxt->comp_mask;
	const osm_physp_t *const p_req_physp = p_ctxt->p_req_physp;
	osm_port_t *p_port;
	uint16_t min_lid_ho, max_lid_ho;
	uint16_t min_block, max_block, block;
	const osm_physp_t *p_physp;

	/* In switches, the port guid is the node guid. */
	p_port = osm_get_port_by_guid(sa->p_subn,
				      p_sw->p_node->node_info.port_guid);
	if (!p_port) {
		OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 4405: "
			"Failed to find Port by Node Guid:0x%016" PRIx64
			"\n", cl_ntoh64(p_sw->p_node->node_info.node_guid));
		return;
	}

	/* check that the requester physp and the current physp are under
	   the same partition. */
	p_physp = p_port->p_physp;
	if (!p_physp) {
		OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 4406: "
			"Failed to find default physical Port by Node Guid:0x%016"
			PRIx64 "\n",
			cl_ntoh64(p_sw->p_node->node_info.node_guid));
		return;
	}
	if (!osm_physp_share_pkey(sa->p_log, p_req_physp,
				  p_physp, sa->p_subn->opt.allow_both_pkeys))
		return;

	/* get the port 0 of the switch */
	osm_port_get_lid_range_ho(p_port, &min_lid_ho, &max_lid_ho);

	/* compare the lids - if required */
	if (comp_mask & IB_LFTR_COMPMASK_LID) {
		OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
			"Comparing lid:%u to port lid range: %u .. %u\n",
			cl_ntoh16(p_rcvd_rec->lid), min_lid_ho, max_lid_ho);
		/* ok we are ready for range check */
		if (min_lid_ho > cl_ntoh16(p_rcvd_rec->lid) ||
		    max_lid_ho < cl_ntoh16(p_rcvd_rec->lid))
			return;
	}

	/* now we need to decide which blocks to output */
	max_block = osm_switch_get_max_block_id_in_use(p_sw);
	if (comp_mask & IB_LFTR_COMPMASK_BLOCK) {
		min_block = cl_ntoh16(p_rcvd_rec->block_num);
		if (min_block > max_block)
			return;
		max_block = min_block;
	} else			/* use as many blocks as "in use" */
		min_block = 0;

	/* so we can add these blocks one by one ... */
	for (block = min_block; block <= max_block; block++)
		lftr_rcv_new_lftr(sa, p_sw, p_ctxt->p_list,
				  osm_port_get_base_lid(p_port), block);
}