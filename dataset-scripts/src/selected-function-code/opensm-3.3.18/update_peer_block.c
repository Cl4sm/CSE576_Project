static int update_peer_block(osm_log_t * p_log, osm_sm_t * sm,
			     osm_physp_t * peer,
			     osm_pkey_tbl_t * p_peer_pkey_tbl,
			     ib_pkey_table_t * new_peer_block,
			     uint16_t peer_block_idx, osm_node_t * p_node)
{
	int ret = 0;
	ib_pkey_table_t *peer_block;

	peer_block = osm_pkey_tbl_block_get(p_peer_pkey_tbl, peer_block_idx);
	if (!peer_block ||
	    memcmp(peer_block, new_peer_block, sizeof(*peer_block))) {
		if (pkey_mgr_update_pkey_entry(sm, peer, new_peer_block,
					       peer_block_idx) != IB_SUCCESS) {
			OSM_LOG(p_log, OSM_LOG_ERROR, "ERR 0509: "
				"pkey_mgr_update_pkey_entry() failed to update "
				"pkey table block %u for node 0x%016"
				PRIx64 " port %u (%s)\n",
				peer_block_idx,
				cl_ntoh64(osm_node_get_node_guid(p_node)),
				osm_physp_get_port_num(peer),
				p_node->print_desc);
			ret = -1;
		}
	}

	return ret;
}