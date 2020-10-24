static int set_lft_block(IN osm_switch_t *p_sw, IN osm_ucast_mgr_t *p_mgr,
			 IN uint16_t block_id_ho)
{
	osm_madw_context_t context;
	osm_dr_path_t *p_path;
	osm_physp_t *p_physp;
	ib_api_status_t status;

	/*
	   Send linear forwarding table blocks to the switch
	   as long as the switch indicates it has blocks needing
	   configuration.
	 */
	if (!p_sw->new_lft) {
		/* any routing should provide the new_lft */
		CL_ASSERT(p_mgr->p_subn->opt.use_ucast_cache &&
			  p_mgr->cache_valid && !p_sw->need_update);
		return -1;
	}

	p_physp = osm_node_get_physp_ptr(p_sw->p_node, 0);
	if (!p_physp)
		return -1;

	p_path = osm_physp_get_dr_path_ptr(p_physp);

	context.lft_context.node_guid = osm_node_get_node_guid(p_sw->p_node);
	context.lft_context.set_method = TRUE;

	if (!p_sw->need_update && !p_mgr->p_subn->need_update &&
	    !memcmp(p_sw->new_lft + block_id_ho * IB_SMP_DATA_SIZE,
		    p_sw->lft + block_id_ho * IB_SMP_DATA_SIZE,
		    IB_SMP_DATA_SIZE))
		return 0;

	/*
	 * Zero the stored LFT block, so in case the MAD will end up
	 * with error, we will resend it in the next sweep.
	 */
	memset(p_sw->lft + block_id_ho * IB_SMP_DATA_SIZE, 0,
	       IB_SMP_DATA_SIZE);

	OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
		"Writing FT block %u to switch 0x%" PRIx64 "\n", block_id_ho,
		cl_ntoh64(context.lft_context.node_guid));

	status = osm_req_set(p_mgr->sm, p_path,
			     p_sw->new_lft + block_id_ho * IB_SMP_DATA_SIZE,
			     IB_SMP_DATA_SIZE, IB_MAD_ATTR_LIN_FWD_TBL,
			     cl_hton32(block_id_ho), FALSE,
			     ib_port_info_get_m_key(&p_physp->port_info),
			     CL_DISP_MSGID_NONE, &context);

	if (status != IB_SUCCESS) {
		OSM_LOG(p_mgr->p_log, OSM_LOG_ERROR, "ERR 3A10: "
			"Sending linear fwd. tbl. block failed (%s)\n",
			ib_get_err_str(status));
		return -1;
	}

	return 0;
}