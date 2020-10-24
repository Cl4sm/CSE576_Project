static qos_mad_item_t *osm_qos_mad_create(IN osm_sm_t * sm,
					  IN osm_physp_t * p,
					  IN uint32_t data_size,
					  IN uint8_t * p_data,
					  IN ib_net16_t attr_id,
					  IN uint32_t attr_mod)

{
	qos_mad_item_t *p_mad;
	osm_madw_context_t context;
	osm_madw_t *p_madw;
	osm_node_t *p_node;
	osm_physp_t *physp0;
	ib_net64_t m_key;

	p_node = osm_physp_get_node_ptr(p);
	if (osm_node_get_type(p_node) == IB_NODE_TYPE_SWITCH &&
	    osm_physp_get_port_num(p) != 0) {
		physp0 = osm_node_get_physp_ptr(p_node, 0);
		m_key = ib_port_info_get_m_key(&physp0->port_info);
	} else
		m_key = ib_port_info_get_m_key(&p->port_info);

	switch (attr_id){
	case IB_MAD_ATTR_SLVL_TABLE:
		context.slvl_context.node_guid = osm_node_get_node_guid(p_node);
		context.slvl_context.port_guid = osm_physp_get_port_guid(p);
		context.slvl_context.set_method = TRUE;
		break;
	case IB_MAD_ATTR_VL_ARBITRATION:
		context.vla_context.node_guid = osm_node_get_node_guid(p_node);
		context.vla_context.port_guid = osm_physp_get_port_guid(p);
		context.vla_context.set_method = TRUE;
		break;
	default:
		return NULL;
	}

	p_mad = (qos_mad_item_t *) malloc(sizeof(*p_mad));
	if (!p_mad)
		return NULL;

	memset(p_mad, 0, sizeof(*p_mad));

	p_madw = osm_prepare_req_set(sm, osm_physp_get_dr_path_ptr(p),
				     p_data, data_size,
				     attr_id, cl_hton32(attr_mod),
				     FALSE, m_key,
				     CL_DISP_MSGID_NONE, &context);

	if (p_madw == NULL) {
		free(p_mad);
		return NULL;
	}
	p_mad->p_madw = p_madw;
	return p_mad;
}