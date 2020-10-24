static int disable_port(osm_sm_t *sm, osm_physp_t *p)
{
	uint8_t payload[IB_SMP_DATA_SIZE];
	osm_madw_context_t context;
	ib_port_info_t *pi = (ib_port_info_t *)payload;
	osm_physp_t *physp0;
	osm_port_t *p_port;
	ib_net64_t m_key;
	ib_api_status_t status;

	/* select the nearest port to master opensm */
	if (p->p_remote_physp &&
	    p->dr_path.hop_count > p->p_remote_physp->dr_path.hop_count)
		p = p->p_remote_physp;

	/* If trap 131, might want to disable peer port if available */
	/* but peer port has been observed not to respond to SM requests */

	memcpy(payload, &p->port_info, sizeof(ib_port_info_t));

	/* Set port to disabled/down */
	ib_port_info_set_port_state(pi, IB_LINK_DOWN);
	ib_port_info_set_port_phys_state(IB_PORT_PHYS_STATE_DISABLED, pi);

	/* Issue set of PortInfo */
	context.pi_context.node_guid = osm_node_get_node_guid(p->p_node);
	context.pi_context.port_guid = osm_physp_get_port_guid(p);
	context.pi_context.set_method = TRUE;
	context.pi_context.light_sweep = FALSE;
	context.pi_context.active_transition = FALSE;
	context.pi_context.client_rereg = FALSE;
	if (osm_node_get_type(p->p_node) == IB_NODE_TYPE_SWITCH &&
	    osm_physp_get_port_num(p) != 0) {
		physp0 = osm_node_get_physp_ptr(p->p_node, 0);
		m_key = ib_port_info_get_m_key(&physp0->port_info);
	} else
		m_key = ib_port_info_get_m_key(&p->port_info);

	if (osm_node_get_type(p->p_node) != IB_NODE_TYPE_SWITCH) {
		if (!pi->base_lid) {
			p_port = osm_get_port_by_guid(sm->p_subn,
						      osm_physp_get_port_guid(p));
			pi->base_lid = p_port->lid;
		}
		pi->master_sm_base_lid = sm->p_subn->sm_base_lid;
	}

	status = osm_req_set(sm, osm_physp_get_dr_path_ptr(p),
			   payload, sizeof(payload), IB_MAD_ATTR_PORT_INFO,
			   cl_hton32(osm_physp_get_port_num(p)),
			   FALSE, m_key,
			   CL_DISP_MSGID_NONE, &context);
	return status;
}