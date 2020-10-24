static void sa_pir_create(IN osm_sa_t * sa, IN const osm_physp_t * p_physp,
			  IN osm_pir_search_ctxt_t * p_ctxt)
{
	uint8_t lmc;
	uint16_t max_lid_ho;
	uint16_t base_lid_ho;
	uint16_t match_lid_ho;
	osm_physp_t *p_node_physp;

	OSM_LOG_ENTER(sa->p_log);

	if (p_physp->p_node->sw) {
		p_node_physp = osm_node_get_physp_ptr(p_physp->p_node, 0);
		base_lid_ho = cl_ntoh16(osm_physp_get_base_lid(p_node_physp));
		lmc =
		    osm_switch_sp0_is_lmc_capable(p_physp->p_node->sw,
						  sa->p_subn) ?
		    osm_physp_get_lmc(p_node_physp) : 0;
	} else {
		lmc = osm_physp_get_lmc(p_physp);
		base_lid_ho = cl_ntoh16(osm_physp_get_base_lid(p_physp));
	}
	max_lid_ho = (uint16_t) (base_lid_ho + (1 << lmc) - 1);

	if (p_ctxt->comp_mask & IB_PIR_COMPMASK_LID) {
		match_lid_ho = cl_ntoh16(p_ctxt->p_rcvd_rec->lid);

		/*
		   We validate that the lid belongs to this node.
		 */
		OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
			"Comparing LID: %u <= %u <= %u\n",
			base_lid_ho, match_lid_ho, max_lid_ho);

		if (match_lid_ho < base_lid_ho || match_lid_ho > max_lid_ho)
			goto Exit;
	}

	pir_rcv_new_pir(sa, p_physp, p_ctxt, cl_hton16(base_lid_ho));

Exit:
	OSM_LOG_EXIT(sa->p_log);
}