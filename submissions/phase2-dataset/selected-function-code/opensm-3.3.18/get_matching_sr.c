static void get_matching_sr(IN cl_list_item_t * p_list_item, IN void *context)
{
	osm_sr_search_ctxt_t *p_ctxt = context;
	osm_svcr_t *p_svcr = (osm_svcr_t *) p_list_item;
	osm_sa_item_t *p_sr_pool_item;
	osm_sr_match_item_t *p_sr_item = p_ctxt->p_sr_item;
	ib_net64_t comp_mask = p_sr_item->comp_mask;
	const osm_physp_t *p_req_physp = p_ctxt->p_req_physp;

	if (comp_mask & IB_SR_COMPMASK_SID) {
		if (p_sr_item->p_service_rec->service_id !=
		    p_svcr->service_record.service_id)
			return;
	}
	if (comp_mask & IB_SR_COMPMASK_SGID) {
		if (memcmp(&p_sr_item->p_service_rec->service_gid,
			   &p_svcr->service_record.service_gid,
			   sizeof(p_svcr->service_record.service_gid)) != 0)
			return;
	}
	if (comp_mask & IB_SR_COMPMASK_SPKEY) {
		if (p_sr_item->p_service_rec->service_pkey !=
		    p_svcr->service_record.service_pkey)
			return;
	}

	if (comp_mask & IB_SR_COMPMASK_SKEY) {
		if (memcmp(p_sr_item->p_service_rec->service_key,
			   p_svcr->service_record.service_key,
			   16 * sizeof(uint8_t)))
			return;
	}
	if (comp_mask & IB_SR_COMPMASK_SNAME) {
		if (memcmp(p_sr_item->p_service_rec->service_name,
			   p_svcr->service_record.service_name,
			   sizeof(p_svcr->service_record.service_name)) != 0)
			return;
	}
	if (comp_mask & IB_SR_COMPMASK_SDATA8_0) {
		if (p_sr_item->p_service_rec->service_data8[0] !=
		    p_svcr->service_record.service_data8[0])
			return;
	}

	if (comp_mask & IB_SR_COMPMASK_SDATA8_1) {
		if (p_sr_item->p_service_rec->service_data8[1] !=
		    p_svcr->service_record.service_data8[1])
			return;
	}
	if (comp_mask & IB_SR_COMPMASK_SDATA8_2) {
		if (p_sr_item->p_service_rec->service_data8[2] !=
		    p_svcr->service_record.service_data8[2])
			return;
	}
	if (comp_mask & IB_SR_COMPMASK_SDATA8_3) {
		if (p_sr_item->p_service_rec->service_data8[3] !=
		    p_svcr->service_record.service_data8[3])
			return;
	}
	if (comp_mask & IB_SR_COMPMASK_SDATA8_4) {
		if (p_sr_item->p_service_rec->service_data8[4] !=
		    p_svcr->service_record.service_data8[4])
			return;
	}
	if (comp_mask & IB_SR_COMPMASK_SDATA8_5) {
		if (p_sr_item->p_service_rec->service_data8[5] !=
		    p_svcr->service_record.service_data8[5])
			return;
	}
	if (comp_mask & IB_SR_COMPMASK_SDATA8_6) {
		if (p_sr_item->p_service_rec->service_data8[6] !=
		    p_svcr->service_record.service_data8[6])
			return;
	}

	if (comp_mask & IB_SR_COMPMASK_SDATA8_7) {
		if (p_sr_item->p_service_rec->service_data8[7] !=
		    p_svcr->service_record.service_data8[7])
			return;
	}

	if (comp_mask & IB_SR_COMPMASK_SDATA8_8) {
		if (p_sr_item->p_service_rec->service_data8[8] !=
		    p_svcr->service_record.service_data8[8])
			return;
	}

	if (comp_mask & IB_SR_COMPMASK_SDATA8_9) {
		if (p_sr_item->p_service_rec->service_data8[9] !=
		    p_svcr->service_record.service_data8[9])
			return;
	}

	if (comp_mask & IB_SR_COMPMASK_SDATA8_10) {
		if (p_sr_item->p_service_rec->service_data8[10] !=
		    p_svcr->service_record.service_data8[10])
			return;
	}

	if (comp_mask & IB_SR_COMPMASK_SDATA8_11) {
		if (p_sr_item->p_service_rec->service_data8[11] !=
		    p_svcr->service_record.service_data8[11])
			return;
	}

	if (comp_mask & IB_SR_COMPMASK_SDATA8_12) {
		if (p_sr_item->p_service_rec->service_data8[12] !=
		    p_svcr->service_record.service_data8[12])
			return;
	}
	if (comp_mask & IB_SR_COMPMASK_SDATA8_13) {
		if (p_sr_item->p_service_rec->service_data8[13] !=
		    p_svcr->service_record.service_data8[13])
			return;
	}
	if (comp_mask & IB_SR_COMPMASK_SDATA8_14) {
		if (p_sr_item->p_service_rec->service_data8[14] !=
		    p_svcr->service_record.service_data8[14])
			return;
	}
	if (comp_mask & IB_SR_COMPMASK_SDATA8_15) {
		if (p_sr_item->p_service_rec->service_data8[15] !=
		    p_svcr->service_record.service_data8[15])
			return;
	}
	if (comp_mask & IB_SR_COMPMASK_SDATA16_0) {
		if (p_sr_item->p_service_rec->service_data16[0] !=
		    p_svcr->service_record.service_data16[0])
			return;
	}
	if (comp_mask & IB_SR_COMPMASK_SDATA16_1) {
		if (p_sr_item->p_service_rec->service_data16[1] !=
		    p_svcr->service_record.service_data16[1])
			return;
	}
	if (comp_mask & IB_SR_COMPMASK_SDATA16_2) {
		if (p_sr_item->p_service_rec->service_data16[2] !=
		    p_svcr->service_record.service_data16[2])
			return;
	}
	if (comp_mask & IB_SR_COMPMASK_SDATA16_3) {
		if (p_sr_item->p_service_rec->service_data16[3] !=
		    p_svcr->service_record.service_data16[3])
			return;
	}
	if (comp_mask & IB_SR_COMPMASK_SDATA16_4) {
		if (p_sr_item->p_service_rec->service_data16[4] !=
		    p_svcr->service_record.service_data16[4])
			return;
	}
	if (comp_mask & IB_SR_COMPMASK_SDATA16_5) {
		if (p_sr_item->p_service_rec->service_data16[5] !=
		    p_svcr->service_record.service_data16[5])
			return;
	}
	if (comp_mask & IB_SR_COMPMASK_SDATA16_6) {
		if (p_sr_item->p_service_rec->service_data16[6] !=
		    p_svcr->service_record.service_data16[6])
			return;
	}
	if (comp_mask & IB_SR_COMPMASK_SDATA16_7) {
		if (p_sr_item->p_service_rec->service_data16[7] !=
		    p_svcr->service_record.service_data16[7])
			return;
	}
	if (comp_mask & IB_SR_COMPMASK_SDATA32_0) {
		if (p_sr_item->p_service_rec->service_data32[0] !=
		    p_svcr->service_record.service_data32[0])
			return;
	}
	if (comp_mask & IB_SR_COMPMASK_SDATA32_1) {
		if (p_sr_item->p_service_rec->service_data32[1] !=
		    p_svcr->service_record.service_data32[1])
			return;
	}
	if (comp_mask & IB_SR_COMPMASK_SDATA32_2) {
		if (p_sr_item->p_service_rec->service_data32[2] !=
		    p_svcr->service_record.service_data32[2])
			return;
	}
	if (comp_mask & IB_SR_COMPMASK_SDATA32_3) {
		if (p_sr_item->p_service_rec->service_data32[3] !=
		    p_svcr->service_record.service_data32[3])
			return;
	}

	if (comp_mask & IB_SR_COMPMASK_SDATA64_0) {
		if (p_sr_item->p_service_rec->service_data64[0] !=
		    p_svcr->service_record.service_data64[0])
			return;
	}
	if (comp_mask & IB_SR_COMPMASK_SDATA64_1) {
		if (p_sr_item->p_service_rec->service_data64[1] !=
		    p_svcr->service_record.service_data64[1])
			return;
	}

	/* Check that the requester port has the pkey which is the service_pkey.
	   If not - then it cannot receive this ServiceRecord. */
	/* The check is relevant only if the service_pkey is valid */
	if (!ib_pkey_is_invalid(p_svcr->service_record.service_pkey)) {
		if (!osm_physp_has_pkey(p_sr_item->sa->p_log,
					p_svcr->service_record.service_pkey,
					p_req_physp)) {
			OSM_LOG(p_sr_item->sa->p_log, OSM_LOG_VERBOSE,
				"requester port doesn't have the service_pkey: 0x%X\n",
				cl_ntoh16(p_svcr->service_record.service_pkey));
			return;
		}
	}

	p_sr_pool_item = malloc(SA_SR_RESP_SIZE);
	if (p_sr_pool_item == NULL) {
		OSM_LOG(p_sr_item->sa->p_log, OSM_LOG_ERROR, "ERR 2408: "
			"Unable to acquire Service Record from pool\n");
		goto Exit;
	}

	p_sr_pool_item->resp.service_rec = p_svcr->service_record;

	cl_qlist_insert_tail(&p_sr_item->sr_list, &p_sr_pool_item->list_item);

Exit:
	return;
}