static void get_pkey_table(IN osm_log_t * p_log, IN osm_sm_t * sm,
			   IN osm_node_t * p_node, IN osm_physp_t * p_physp)
{

	osm_madw_context_t context;
	ib_api_status_t status;
	osm_dr_path_t path;
	osm_physp_t *physp0;
	ib_net64_t m_key;
	uint8_t port_num;
	uint16_t block_num, max_blocks;
	uint32_t attr_mod_ho;

	OSM_LOG_ENTER(p_log);

	path = *osm_physp_get_dr_path_ptr(p_physp);

	context.pkey_context.node_guid = osm_node_get_node_guid(p_node);
	context.pkey_context.port_guid = osm_physp_get_port_guid(p_physp);
	context.pkey_context.set_method = FALSE;

	port_num = p_physp->port_num;

	if (!p_node->sw || port_num == 0)
		/* The maximum blocks is defined by the node info partition cap
		   for CA, router, and switch management ports. */
		max_blocks =
		    (cl_ntoh16(p_node->node_info.partition_cap) +
		     IB_NUM_PKEY_ELEMENTS_IN_BLOCK - 1)
		    / IB_NUM_PKEY_ELEMENTS_IN_BLOCK;
	else {
		/* This is a switch, and not a management port. The maximum blocks
		   is defined in the switch info partition enforcement cap. */

		/* Check for IBM eHCA firmware defect in reporting partition enforcement cap */
		if (cl_ntoh32(ib_node_info_get_vendor_id(&p_node->node_info)) ==
		    IBM_VENDOR_ID)
			p_node->sw->switch_info.enforce_cap = 0;

		/* Bail out if this is a switch with no partition enforcement capability */
		if (cl_ntoh16(p_node->sw->switch_info.enforce_cap) == 0)
			goto Exit;

		max_blocks = (cl_ntoh16(p_node->sw->switch_info.enforce_cap) +
			      IB_NUM_PKEY_ELEMENTS_IN_BLOCK -
			      1) / IB_NUM_PKEY_ELEMENTS_IN_BLOCK;
	}

	p_physp->pkeys.rcv_blocks_cnt = max_blocks;
	for (block_num = 0; block_num < max_blocks; block_num++) {
		if (osm_node_get_type(p_node) != IB_NODE_TYPE_SWITCH ||
		    osm_physp_get_port_num(p_physp) == 0) {
			attr_mod_ho = block_num;
			m_key = ib_port_info_get_m_key(&p_physp->port_info);
		} else {
			attr_mod_ho = block_num | (port_num << 16);
			physp0 = osm_node_get_physp_ptr(p_node, 0);
			m_key = ib_port_info_get_m_key(&physp0->port_info);
		}
		status = osm_req_get(sm, &path, IB_MAD_ATTR_P_KEY_TABLE,
				     cl_hton32(attr_mod_ho), FALSE,
				     m_key, CL_DISP_MSGID_NONE, &context);

		if (status != IB_SUCCESS) {
			OSM_LOG(p_log, OSM_LOG_ERROR, "ERR 0F12: "
				"Failure initiating PKeyTable request (%s)\n",
				ib_get_err_str(status));
			goto Exit;
		}
	}

Exit:
	OSM_LOG_EXIT(p_log);
}