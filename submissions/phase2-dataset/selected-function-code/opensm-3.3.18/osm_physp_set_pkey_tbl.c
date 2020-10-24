void osm_physp_set_pkey_tbl(IN osm_log_t * p_log, IN const osm_subn_t * p_subn,
			    IN osm_physp_t * p_physp,
			    IN ib_pkey_table_t * p_pkey_tbl,
			    IN uint16_t block_num,
			    IN boolean_t is_set)
{
	uint16_t max_blocks;

	CL_ASSERT(p_pkey_tbl);
	/*
	   (14.2.5.7) - the block number valid values are 0-2047, and are
	   further limited by the size of the P_Key table specified by
	   the PartitionCap on the node.
	 */
	if (!p_physp->p_node->sw || p_physp->port_num == 0)
		/*
		   The maximum blocks is defined in the node info: partition cap
		   for CA, router, and switch management ports.
		 */
		max_blocks =
		    (cl_ntoh16(p_physp->p_node->node_info.partition_cap) +
		     IB_NUM_PKEY_ELEMENTS_IN_BLOCK - 1)
		    / IB_NUM_PKEY_ELEMENTS_IN_BLOCK;
	else
		/*
		   This is a switch, and not a management port. The maximum
		   blocks is defined in the switch info: partition enforcement
		   cap.
		 */
		max_blocks =
		    (cl_ntoh16(p_physp->p_node->sw->switch_info.enforce_cap) +
		     IB_NUM_PKEY_ELEMENTS_IN_BLOCK -
		     1) / IB_NUM_PKEY_ELEMENTS_IN_BLOCK;

	if (block_num >= max_blocks) {
		OSM_LOG(p_log, OSM_LOG_ERROR, "ERR 4108: "
			"Got illegal update for block number:%u max:%u "
			"for GUID: %" PRIx64 " port number:%u\n",
			block_num, max_blocks,
			cl_ntoh64(p_physp->p_node->node_info.node_guid),
			p_physp->port_num);
		return;
	}

	/* decrement block received counter */
	if(!is_set)
		p_physp->pkeys.rcv_blocks_cnt--;
	osm_pkey_tbl_set(&p_physp->pkeys, block_num, p_pkey_tbl,
			 p_subn->opt.allow_both_pkeys);
}