static int mcast_mgr_set_mft_block(osm_sm_t * sm, IN osm_switch_t * p_sw,
				   uint32_t block_num, uint32_t position)
{
	osm_node_t *p_node;
	osm_physp_t *p_physp;
	osm_dr_path_t *p_path;
	osm_madw_context_t context;
	ib_api_status_t status;
	uint32_t block_id_ho;
	osm_mcast_tbl_t *p_tbl;
	ib_net16_t block[IB_MCAST_BLOCK_SIZE];
	int ret = 0;

	CL_ASSERT(sm);

	OSM_LOG_ENTER(sm->p_log);

	CL_ASSERT(p_sw);

	p_node = p_sw->p_node;

	CL_ASSERT(p_node);

	p_physp = osm_node_get_physp_ptr(p_node, 0);
	p_path = osm_physp_get_dr_path_ptr(p_physp);

	/*
	   Send multicast forwarding table blocks to the switch
	   as long as the switch indicates it has blocks needing
	   configuration.
	 */

	context.mft_context.node_guid = osm_node_get_node_guid(p_node);
	context.mft_context.set_method = TRUE;

	p_tbl = osm_switch_get_mcast_tbl_ptr(p_sw);

	if (osm_mcast_tbl_get_block(p_tbl, (uint16_t) block_num,
				    (uint8_t) position, block)) {
		block_id_ho = block_num + (position << 28);

		OSM_LOG(sm->p_log, OSM_LOG_DEBUG,
			"Writing MFT block %u position %u to switch 0x%" PRIx64
			"\n", block_num, position,
			cl_ntoh64(context.mft_context.node_guid));

		status = osm_req_set(sm, p_path, (void *)block, sizeof(block),
				     IB_MAD_ATTR_MCAST_FWD_TBL,
				     cl_hton32(block_id_ho), FALSE,
				     ib_port_info_get_m_key(&p_physp->port_info),
				     CL_DISP_MSGID_NONE, &context);
		if (status != IB_SUCCESS) {
			OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 0A02: "
				"Sending multicast fwd. tbl. block 0x%X to %s "
				"failed (%s)\n", block_id_ho,
				p_node->print_desc, ib_get_err_str(status));
			ret = -1;
		}
	}

	OSM_LOG_EXIT(sm->p_log);
	return ret;
}