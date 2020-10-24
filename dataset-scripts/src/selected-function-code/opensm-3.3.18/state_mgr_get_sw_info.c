static void state_mgr_get_sw_info(IN cl_map_item_t * p_object, IN void *context)
{
	osm_node_t *p_node;
	osm_physp_t *p_physp;
	osm_dr_path_t *p_dr_path;
	osm_madw_context_t mad_context;
	osm_switch_t *const p_sw = (osm_switch_t *) p_object;
	osm_sm_t *sm = context;
	ib_api_status_t status;

	OSM_LOG_ENTER(sm->p_log);

	p_node = p_sw->p_node;
	p_physp = osm_node_get_physp_ptr(p_node, 0);
	p_dr_path = osm_physp_get_dr_path_ptr(p_physp);

	memset(&mad_context, 0, sizeof(mad_context));

	mad_context.si_context.node_guid = osm_node_get_node_guid(p_node);
	mad_context.si_context.set_method = FALSE;
	mad_context.si_context.light_sweep = TRUE;
	mad_context.si_context.lft_top_change = FALSE;

	status = osm_req_get(sm, p_dr_path, IB_MAD_ATTR_SWITCH_INFO, 0,
			     FALSE, ib_port_info_get_m_key(&p_physp->port_info),
			     OSM_MSG_LIGHT_SWEEP_FAIL, &mad_context);
	if (status != IB_SUCCESS)
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 3304: "
			"Request for SwitchInfo from 0x%" PRIx64 " failed (%s)\n",
			cl_ntoh64(osm_node_get_node_guid(p_node)),
			ib_get_err_str(status));

	OSM_LOG_EXIT(sm->p_log);
}