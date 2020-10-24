static int pkey_mgr_update_peer_port(osm_log_t * p_log, osm_sm_t * sm,
				     const osm_subn_t * p_subn,
				     const osm_port_t * const p_port,
				     osm_partition_enforce_type_enum enforce_type)
{
	osm_physp_t *p_physp, *peer;
	osm_node_t *p_node;
	ib_pkey_table_t *block;
	const osm_pkey_tbl_t *p_pkey_tbl;
	osm_pkey_tbl_t *p_peer_pkey_tbl;
	uint16_t block_index, peer_block_idx;
	uint16_t peer_max_blocks;
	uint16_t last_index;
	ib_pkey_table_t new_peer_block;
	uint16_t pkey_idx, peer_pkey_idx;
	ib_net16_t pkey, full_pkey;
	int ret = 0, loop_exit = 0;

	p_physp = p_port->p_physp;
	if (!p_physp)
		return -1;
	peer = osm_physp_get_remote(p_physp);
	if (!peer)
		return -1;
	p_node = osm_physp_get_node_ptr(peer);
	if (!p_node->sw || !p_node->sw->switch_info.enforce_cap)
		return 0;

	if (enforce_type == OSM_PARTITION_ENFORCE_TYPE_OFF) {
		pkey_mgr_enforce_partition(p_log, sm, peer, OSM_PARTITION_ENFORCE_TYPE_OFF);
		return ret;
	}

	p_pkey_tbl = osm_physp_get_pkey_tbl(p_physp);
	peer_max_blocks = pkey_mgr_get_physp_max_blocks(peer);
	p_peer_pkey_tbl = &peer->pkeys;
	peer_block_idx = 0;
	peer_pkey_idx = 0;
	for (block_index = 0; block_index < p_pkey_tbl->used_blocks;
	     block_index++) {
		if (loop_exit)
			break;
		block = osm_pkey_tbl_new_block_get(p_pkey_tbl, block_index);
		if (!block)
			continue;
		for (pkey_idx = 0; pkey_idx < IB_NUM_PKEY_ELEMENTS_IN_BLOCK;
		     pkey_idx++) {
			pkey = block->pkey_entry[pkey_idx];
			if (ib_pkey_is_invalid(pkey))
				continue;
			if (!ib_pkey_is_full_member(pkey)) {
				full_pkey = pkey | IB_PKEY_TYPE_MASK;
				if (new_pkey_exists(&p_physp->pkeys, full_pkey))
					continue;
			}
			new_peer_block.pkey_entry[peer_pkey_idx] = pkey;
			if (peer_block_idx >= peer_max_blocks) {
				loop_exit = 1;
				break;
			}
			if (++peer_pkey_idx == IB_NUM_PKEY_ELEMENTS_IN_BLOCK) {
				if (update_peer_block(p_log, sm, peer,
						      p_peer_pkey_tbl,
						      &new_peer_block,
						      peer_block_idx, p_node))
					ret = -1;
				peer_pkey_idx = 0;
				peer_block_idx++;
			}
		}
	}

	if (peer_block_idx < peer_max_blocks) {
		if (peer_pkey_idx) {
			/* Handle partial last block */
			for (; peer_pkey_idx < IB_NUM_PKEY_ELEMENTS_IN_BLOCK;
			     peer_pkey_idx++)
				new_peer_block.pkey_entry[peer_pkey_idx] = 0;
			if (update_peer_block(p_log, sm, peer, p_peer_pkey_tbl,
					      &new_peer_block, peer_block_idx,
					      p_node))
				ret = -1;
		} else
			peer_block_idx--;

		p_peer_pkey_tbl->used_blocks = peer_block_idx + 1;
		if (p_peer_pkey_tbl->used_blocks == peer_max_blocks) {
			/* Is last used pkey index beyond switch peer port capacity ? */
			if (!last_used_pkey_index(p_port, p_peer_pkey_tbl,
						  &last_index)) {
				last_index += peer_block_idx * IB_NUM_PKEY_ELEMENTS_IN_BLOCK;
				if (cl_ntoh16(p_node->sw->switch_info.enforce_cap) <= last_index) {
					OSM_LOG(p_log, OSM_LOG_ERROR, "ERR 0507: "
						"Not enough pkey entries (%u <= %u) on switch 0x%016"
						PRIx64 " port %u (%s). Clearing Enforcement bit\n",
						cl_ntoh16(p_node->sw->switch_info.enforce_cap),
						last_index,
						cl_ntoh64(osm_node_get_node_guid(p_node)),
						osm_physp_get_port_num(peer),
						p_node->print_desc);
					enforce_type = OSM_PARTITION_ENFORCE_TYPE_OFF;
					ret = -1;
				}
			}
		}
	} else {
		p_peer_pkey_tbl->used_blocks = peer_max_blocks;
		enforce_type = OSM_PARTITION_ENFORCE_TYPE_OFF;
	}

	if (!ret)
		OSM_LOG(p_log, OSM_LOG_DEBUG,
			"Pkey table was successfully updated for node 0x%016"
			PRIx64 " port %u (%s)\n",
			cl_ntoh64(osm_node_get_node_guid(p_node)),
			osm_physp_get_port_num(peer), p_node->print_desc);

	if (pkey_mgr_enforce_partition(p_log, sm, peer, enforce_type))
		ret = -1;

	return ret;
}