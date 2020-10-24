static void mcast_mgr_set_mfttop(IN osm_sm_t * sm, IN osm_switch_t * p_sw)
{
	osm_node_t *p_node;
	osm_dr_path_t *p_path;
	osm_physp_t *p_physp;
	osm_mcast_tbl_t *p_tbl;
	osm_madw_context_t context;
	ib_api_status_t status;
	ib_switch_info_t si;
	ib_net16_t mcast_top;

	OSM_LOG_ENTER(sm->p_log);

	CL_ASSERT(p_sw);

	p_node = p_sw->p_node;

	CL_ASSERT(p_node);

	p_physp = osm_node_get_physp_ptr(p_node, 0);
	p_path = osm_physp_get_dr_path_ptr(p_physp);
	p_tbl = osm_switch_get_mcast_tbl_ptr(p_sw);

	if (sm->p_subn->opt.use_mfttop &&
	    p_physp->port_info.capability_mask & IB_PORT_CAP_HAS_MCAST_FDB_TOP) {
		/*
		   Set the top of the multicast forwarding table.
		 */
		si = p_sw->switch_info;
		if (sm->p_subn->first_time_master_sweep == TRUE)
			mcast_top = cl_hton16(sm->mlids_init_max);
		else {
			if (p_tbl->max_block_in_use == -1)
				mcast_top = cl_hton16(IB_LID_MCAST_START_HO - 1);
			else
				mcast_top = cl_hton16(IB_LID_MCAST_START_HO +
						      (p_tbl->max_block_in_use + 1) * IB_MCAST_BLOCK_SIZE - 1);
		}
		if (mcast_top == si.mcast_top)
			return;

		si.mcast_top = mcast_top;

		OSM_LOG(sm->p_log, OSM_LOG_DEBUG,
			"Setting switch MFT top to MLID 0x%x\n",
			cl_ntoh16(si.mcast_top));

		context.si_context.light_sweep = FALSE;
		context.si_context.node_guid = osm_node_get_node_guid(p_node);
		context.si_context.set_method = TRUE;
		context.si_context.lft_top_change = FALSE;

		status = osm_req_set(sm, p_path, (uint8_t *) & si,
				     sizeof(si), IB_MAD_ATTR_SWITCH_INFO,
				     0, FALSE,
				     ib_port_info_get_m_key(&p_physp->port_info),
				     CL_DISP_MSGID_NONE, &context);

		if (status != IB_SUCCESS)
			OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 0A1B: "
				"Sending SwitchInfo attribute failed (%s)\n",
				ib_get_err_str(status));
	}
}