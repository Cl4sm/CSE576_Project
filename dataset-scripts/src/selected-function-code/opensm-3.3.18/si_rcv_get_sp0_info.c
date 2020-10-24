static void si_rcv_get_sp0_info(IN osm_sm_t * sm, IN osm_node_t * node)
{
	osm_madw_context_t context;
	osm_physp_t *physp;
	ib_api_status_t status;
	int mlnx_epi_supported = 0;

	physp = osm_node_get_physp_ptr(node, 0);

	context.pi_context.node_guid = osm_node_get_node_guid(node);
	context.pi_context.port_guid = osm_physp_get_port_guid(physp);
	context.pi_context.set_method = FALSE;
	context.pi_context.light_sweep = FALSE;
	context.pi_context.active_transition = FALSE;
	context.pi_context.client_rereg = FALSE;

	status = osm_req_get(sm, osm_physp_get_dr_path_ptr(physp),
			     IB_MAD_ATTR_PORT_INFO, 0, TRUE, 0,
			     CL_DISP_MSGID_NONE, &context);
	if (status != IB_SUCCESS)
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 3611: "
			"Failure initiating PortInfo request (%s)\n",
			ib_get_err_str(status));

	if (ib_switch_info_is_enhanced_port0(&node->sw->switch_info) &&
	    sm->p_subn->opt.fdr10) {
		mlnx_epi_supported = is_mlnx_ext_port_info_supported(node->node_info.device_id);
		if (mlnx_epi_supported) {
			status = osm_req_get(sm,
					     osm_physp_get_dr_path_ptr(physp),
					     IB_MAD_ATTR_MLNX_EXTENDED_PORT_INFO,
					     0, TRUE, 0,
					     CL_DISP_MSGID_NONE, &context);
			if (status != IB_SUCCESS)
				OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 3616: "
					"Failure initiating MLNX ExtPortInfo request (%s)\n",
					ib_get_err_str(status));
		}
	}

}