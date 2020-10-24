static void sir_rcv_by_comp_mask(IN cl_map_item_t * p_map_item, IN void *cxt)
{
	const osm_sir_search_ctxt_t *p_ctxt = cxt;
	const osm_switch_t *p_sw = (osm_switch_t *) p_map_item;
	const ib_switch_info_record_t *const p_rcvd_rec = p_ctxt->p_rcvd_rec;
	const osm_physp_t *const p_req_physp = p_ctxt->p_req_physp;
	osm_sa_t *sa = p_ctxt->sa;
	ib_net64_t const comp_mask = p_ctxt->comp_mask;
	ib_net16_t match_lid = 0;

	OSM_LOG_ENTER(p_ctxt->sa->p_log);

	osm_dump_switch_info_v2(p_ctxt->sa->p_log, &p_sw->switch_info,
			        FILE_ID, OSM_LOG_VERBOSE);

	if (comp_mask & IB_SWIR_COMPMASK_LID) {
		match_lid = p_rcvd_rec->lid;
		if (!match_lid)
			goto Exit;
	}

	sir_rcv_create_sir(sa, p_sw, p_ctxt->p_list, match_lid, p_req_physp);

Exit:
	OSM_LOG_EXIT(p_ctxt->sa->p_log);
}