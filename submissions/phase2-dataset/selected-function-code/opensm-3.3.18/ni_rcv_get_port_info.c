static void ni_rcv_get_port_info(IN osm_sm_t * sm, IN osm_node_t * node,
				 IN const osm_madw_t * madw)
{
	osm_madw_context_t context;
	osm_physp_t *physp;
	ib_node_info_t *ni;
	unsigned port;
	ib_api_status_t status;
	int mlnx_epi_supported = 0;

	ni = ib_smp_get_payload_ptr(osm_madw_get_smp_ptr(madw));

	port = ib_node_info_get_local_port_num(ni);

	if (sm->p_subn->opt.fdr10)
		mlnx_epi_supported = is_mlnx_ext_port_info_supported(ni->device_id);

	physp = osm_node_get_physp_ptr(node, port);

	context.pi_context.node_guid = osm_node_get_node_guid(node);
	context.pi_context.port_guid = osm_physp_get_port_guid(physp);
	context.pi_context.set_method = FALSE;
	context.pi_context.light_sweep = FALSE;
	context.pi_context.active_transition = FALSE;
	context.pi_context.client_rereg = FALSE;

	status = osm_req_get(sm, osm_physp_get_dr_path_ptr(physp),
			     IB_MAD_ATTR_PORT_INFO, cl_hton32(port),
			     TRUE, 0, CL_DISP_MSGID_NONE, &context);
	if (status != IB_SUCCESS)
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR OD02: "
			"Failure initiating PortInfo request (%s)\n",
			ib_get_err_str(status));
	if (mlnx_epi_supported) {
		status = osm_req_get(sm,
				     osm_physp_get_dr_path_ptr(physp),
				     IB_MAD_ATTR_MLNX_EXTENDED_PORT_INFO,
				     cl_hton32(port),
				     TRUE, 0, CL_DISP_MSGID_NONE, &context);
		if (status != IB_SUCCESS)
			OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 0D0B: "
				"Failure initiating MLNX ExtPortInfo request (%s)\n",
				ib_get_err_str(status));
	}
}