static void pi_rcv_process_switch_port0(IN osm_sm_t * sm,
					IN osm_node_t * p_node,
					IN osm_physp_t * p_physp,
					IN ib_port_info_t * p_pi)
{
	ib_api_status_t status;
	osm_madw_context_t context;
	uint8_t port, num_ports;

	OSM_LOG_ENTER(sm->p_log);

	num_ports = osm_node_get_num_physp(p_node);

	context.pi_context.node_guid = osm_node_get_node_guid(p_node);
	context.pi_context.port_guid = osm_physp_get_port_guid(p_physp);
	context.pi_context.set_method = FALSE;
	context.pi_context.light_sweep = FALSE;
	context.pi_context.active_transition = FALSE;
	context.pi_context.client_rereg = FALSE;

	for (port = 1; port < num_ports; port++) {
		status = osm_req_get(sm, osm_physp_get_dr_path_ptr(p_physp),
				     IB_MAD_ATTR_PORT_INFO, cl_hton32(port),
				     FALSE,
				     ib_port_info_get_m_key(&p_physp->port_info),
				     CL_DISP_MSGID_NONE, &context);
		if (status != IB_SUCCESS)
			OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 0F16: "
				"Failure initiating PortInfo request (%s)\n",
				ib_get_err_str(status));
	}

	if (p_physp->need_update)
		sm->p_subn->ignore_existing_lfts = TRUE;

	pi_rcv_check_and_fix_lid(sm->p_log, p_pi, p_physp);

	/*
	   Update the PortInfo attribute.
	 */
	osm_physp_set_port_info(p_physp, p_pi, sm);

	/* Determine if base switch port 0 */
	if (p_node->sw &&
	    !ib_switch_info_is_enhanced_port0(&p_node->sw->switch_info))
		/* PortState is not used on BSP0 but just in case it is DOWN */
		p_physp->port_info = *p_pi;
	pi_rcv_process_endport(sm, p_physp, p_pi);
	OSM_LOG_EXIT(sm->p_log);
}