static ib_api_status_t
pkey_mgr_enforce_partition(IN osm_log_t * p_log, osm_sm_t * sm,
			   IN osm_physp_t * p_physp,
			   IN osm_partition_enforce_type_enum enforce_type)
{
	osm_madw_context_t context;
	uint8_t payload[IB_SMP_DATA_SIZE];
	ib_port_info_t *p_pi;
	ib_net64_t m_key;
	osm_physp_t *physp0;
	ib_api_status_t status;
	uint8_t enforce_bits;

	p_pi = &p_physp->port_info;

	if (enforce_type == OSM_PARTITION_ENFORCE_TYPE_BOTH)
		enforce_bits = 0xc;
	else if (enforce_type == OSM_PARTITION_ENFORCE_TYPE_IN)
		enforce_bits = 0x8;
	else
		enforce_bits = 0x4;

	if ((p_pi->vl_enforce & 0xc) == enforce_bits *
	    (enforce_type != OSM_PARTITION_ENFORCE_TYPE_OFF)) {
		OSM_LOG(p_log, OSM_LOG_DEBUG,
			"No need to update PortInfo for "
			"node 0x%016" PRIx64 " port %u (%s)\n",
			cl_ntoh64(osm_node_get_node_guid
				  (osm_physp_get_node_ptr(p_physp))),
			osm_physp_get_port_num(p_physp),
			p_physp->p_node->print_desc);
		return IB_SUCCESS;
	}

	memcpy(payload, p_pi, sizeof(ib_port_info_t));

	p_pi = (ib_port_info_t *) payload;
	p_pi->vl_enforce &= ~0xc;
	if (enforce_type != OSM_PARTITION_ENFORCE_TYPE_OFF)
		p_pi->vl_enforce |= enforce_bits;

	p_pi->state_info2 = 0;
	ib_port_info_set_port_state(p_pi, IB_LINK_NO_CHANGE);

	physp0 = osm_node_get_physp_ptr(p_physp->p_node, 0);
	m_key = ib_port_info_get_m_key(&physp0->port_info);

	context.pi_context.node_guid =
	    osm_node_get_node_guid(osm_physp_get_node_ptr(p_physp));
	context.pi_context.port_guid = osm_physp_get_port_guid(p_physp);
	context.pi_context.set_method = TRUE;
	context.pi_context.light_sweep = FALSE;
	context.pi_context.active_transition = FALSE;
	context.pi_context.client_rereg = FALSE;

	status = osm_req_set(sm, osm_physp_get_dr_path_ptr(p_physp),
			     payload, sizeof(payload),
			     IB_MAD_ATTR_PORT_INFO,
			     cl_hton32(osm_physp_get_port_num(p_physp)),
			     FALSE, m_key,
			     CL_DISP_MSGID_NONE, &context);
	if (status != IB_SUCCESS)
		OSM_LOG(p_log, OSM_LOG_ERROR, "ERR 0511: "
			"Failed to set PortInfo for "
			"node 0x%016" PRIx64 " port %u (%s)\n",
			cl_ntoh64(osm_node_get_node_guid
				  (osm_physp_get_node_ptr(p_physp))),
			osm_physp_get_port_num(p_physp),
			p_physp->p_node->print_desc);
	else
		OSM_LOG(p_log, OSM_LOG_DEBUG,
			"Set PortInfo for node 0x%016" PRIx64 " port %u (%s)\n",
			cl_ntoh64(osm_node_get_node_guid
				  (osm_physp_get_node_ptr(p_physp))),
			osm_physp_get_port_num(p_physp),
			p_physp->p_node->print_desc);
	return status;
}