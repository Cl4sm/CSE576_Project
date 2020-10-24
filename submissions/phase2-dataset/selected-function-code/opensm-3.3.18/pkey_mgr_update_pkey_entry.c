static ib_api_status_t
pkey_mgr_update_pkey_entry(IN osm_sm_t * sm,
			   IN const osm_physp_t * p_physp,
			   IN const ib_pkey_table_t * block,
			   IN const uint16_t block_index)
{
	osm_madw_context_t context;
	osm_node_t *p_node = osm_physp_get_node_ptr(p_physp);
	osm_physp_t *physp0;
	uint32_t attr_mod;
	ib_net64_t m_key;

	context.pkey_context.node_guid = osm_node_get_node_guid(p_node);
	context.pkey_context.port_guid = osm_physp_get_port_guid(p_physp);
	context.pkey_context.set_method = TRUE;
	attr_mod = block_index;
	if (osm_node_get_type(p_node) == IB_NODE_TYPE_SWITCH &&
	    osm_physp_get_port_num(p_physp) != 0) {
		attr_mod |= osm_physp_get_port_num(p_physp) << 16;
		physp0 = osm_node_get_physp_ptr(p_node, 0);
		m_key = ib_port_info_get_m_key(&physp0->port_info);
	} else
		m_key = ib_port_info_get_m_key(&p_physp->port_info);
	return osm_req_set(sm, osm_physp_get_dr_path_ptr(p_physp),
			   (uint8_t *) block, sizeof(*block),
			   IB_MAD_ATTR_P_KEY_TABLE,
			   cl_hton32(attr_mod), FALSE, m_key,
			   CL_DISP_MSGID_NONE, &context);
}