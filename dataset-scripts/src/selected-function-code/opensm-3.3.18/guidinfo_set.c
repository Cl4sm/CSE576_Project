static void guidinfo_set(IN osm_sa_t *sa, IN osm_port_t *p_port,
			 IN uint8_t block_num)
{
	uint8_t payload[IB_SMP_DATA_SIZE];
	osm_madw_context_t context;
	ib_api_status_t status;

	memcpy(payload,
	       &((*p_port->p_physp->p_guids)[block_num * GUID_TABLE_MAX_ENTRIES]),
	       sizeof(ib_guid_info_t));

	context.gi_context.node_guid = osm_node_get_node_guid(p_port->p_node);
	context.gi_context.port_guid = osm_physp_get_port_guid(p_port->p_physp);
	context.gi_context.set_method = TRUE;
	context.gi_context.port_num = osm_physp_get_port_num(p_port->p_physp);

	status = osm_req_set(sa->sm, osm_physp_get_dr_path_ptr(p_port->p_physp),
			     payload, sizeof(payload), IB_MAD_ATTR_GUID_INFO,
			     cl_hton32((uint32_t)block_num), FALSE,
			     ib_port_info_get_m_key(&p_port->p_physp->port_info),
					            CL_DISP_MSGID_NONE, &context);
	if (status != IB_SUCCESS)
		OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 5109: "
			"Failure initiating GUIDInfo request (%s)\n",
			ib_get_err_str(status));
}