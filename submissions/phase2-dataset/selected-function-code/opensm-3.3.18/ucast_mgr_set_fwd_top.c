static void ucast_mgr_set_fwd_top(IN cl_map_item_t * p_map_item,
				  IN void *cxt)
{
	osm_ucast_mgr_t *p_mgr = cxt;
	osm_switch_t * p_sw = (osm_switch_t *) p_map_item;
	osm_node_t *p_node;
	osm_physp_t *p_physp;
	osm_dr_path_t *p_path;
	osm_madw_context_t context;
	ib_api_status_t status;
	ib_switch_info_t si;
	boolean_t set_swinfo_require = FALSE;
	uint16_t lin_top;
	uint8_t life_state;

	CL_ASSERT(p_mgr);

	OSM_LOG_ENTER(p_mgr->p_log);

	CL_ASSERT(p_sw && p_sw->max_lid_ho);

	p_node = p_sw->p_node;

	CL_ASSERT(p_node);

	if (p_mgr->max_lid < p_sw->max_lid_ho)
		p_mgr->max_lid = p_sw->max_lid_ho;

	p_physp = osm_node_get_physp_ptr(p_node, 0);

	CL_ASSERT(p_physp);

	p_path = osm_physp_get_dr_path_ptr(p_physp);

	/*
	   Set the top of the unicast forwarding table.
	 */
	si = p_sw->switch_info;
	lin_top = cl_hton16(p_sw->max_lid_ho);
	if (lin_top != si.lin_top) {
		set_swinfo_require = TRUE;
		si.lin_top = lin_top;
		context.si_context.lft_top_change = TRUE;
	} else
		context.si_context.lft_top_change = FALSE;

	life_state = si.life_state;
	ib_switch_info_set_life_time(&si, p_mgr->p_subn->opt.packet_life_time);

	if (life_state != si.life_state)
		set_swinfo_require = TRUE;

	if (set_swinfo_require) {
		OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
			"Setting switch FT top to LID %u\n", p_sw->max_lid_ho);

		context.si_context.light_sweep = FALSE;
		context.si_context.node_guid = osm_node_get_node_guid(p_node);
		context.si_context.set_method = TRUE;

		status = osm_req_set(p_mgr->sm, p_path, (uint8_t *) & si,
				     sizeof(si), IB_MAD_ATTR_SWITCH_INFO,
				     0, FALSE,
				     ib_port_info_get_m_key(&p_physp->port_info),
				     CL_DISP_MSGID_NONE, &context);

		if (status != IB_SUCCESS)
			OSM_LOG(p_mgr->p_log, OSM_LOG_ERROR, "ERR 3A06: "
				"Sending SwitchInfo attribute failed (%s)\n",
				ib_get_err_str(status));
	}

	OSM_LOG_EXIT(p_mgr->p_log);
}