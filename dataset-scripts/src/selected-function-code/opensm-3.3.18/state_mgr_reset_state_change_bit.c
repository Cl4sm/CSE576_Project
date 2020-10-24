static void state_mgr_reset_state_change_bit(IN cl_map_item_t * obj,
					     IN void *context)
{
	osm_madw_context_t mad_context;
	osm_switch_t *p_sw = (osm_switch_t *) obj;
	osm_sm_t *sm = context;
	osm_node_t *p_node;
	osm_physp_t *p_physp;
	osm_dr_path_t *p_path;
	ib_api_status_t status;
	ib_switch_info_t si;

	OSM_LOG_ENTER(sm->p_log);

	CL_ASSERT(p_sw);

	p_node = p_sw->p_node;

	CL_ASSERT(p_node);

	p_physp = osm_node_get_physp_ptr(p_node, 0);
	p_path = osm_physp_get_dr_path_ptr(p_physp);

	if (!ib_switch_info_get_state_change(&p_sw->switch_info))
		goto exit;

	si = p_sw->switch_info;

	ib_switch_info_state_change_set(&si);

	OSM_LOG(sm->p_log, OSM_LOG_DEBUG,
		"Resetting PortStateChange on switch GUID 0x%016" PRIx64 "\n",
		cl_ntoh64(osm_node_get_node_guid(p_node)));

	mad_context.si_context.light_sweep = FALSE;
	mad_context.si_context.node_guid = osm_node_get_node_guid(p_node);
	mad_context.si_context.set_method = TRUE;
	mad_context.si_context.lft_top_change = FALSE;

	status = osm_req_set(sm, p_path, (uint8_t *) &si,
			     sizeof(si), IB_MAD_ATTR_SWITCH_INFO,
			     0, FALSE,
			     ib_port_info_get_m_key(&p_physp->port_info),
			     CL_DISP_MSGID_NONE, &mad_context);

	if (status != IB_SUCCESS)
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 332A: "
			"Sending SwitchInfo attribute failed (%s)\n",
			ib_get_err_str(status));

exit:
	OSM_LOG_EXIT(sm->p_log);
}