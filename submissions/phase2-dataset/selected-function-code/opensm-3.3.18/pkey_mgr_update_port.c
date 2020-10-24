static int pkey_mgr_update_port(osm_log_t * p_log, osm_sm_t * sm,
				const osm_port_t * const p_port)
{
	osm_physp_t *p_physp;
	osm_node_t *p_node;
	ib_pkey_table_t *block, *new_block;
	osm_pkey_tbl_t *p_pkey_tbl;
	uint16_t block_index;
	uint8_t pkey_index;
	uint16_t last_free_block_index = 0;
	uint8_t last_free_pkey_index = 0;
	uint16_t num_of_blocks;
	uint16_t max_num_of_blocks;
	ib_api_status_t status;
	osm_pending_pkey_t *p_pending;
	boolean_t found;
	ib_pkey_table_t empty_block;
	int ret = 0, full = 0;
	void *ptr;
	uintptr_t pkey_idx_ptr;
	uint16_t pkey_idx;

	p_physp = p_port->p_physp;
	if (!p_physp)
		return FALSE;

	memset(&empty_block, 0, sizeof(ib_pkey_table_t));

	p_node = osm_physp_get_node_ptr(p_physp);
	p_pkey_tbl = &p_physp->pkeys;
	num_of_blocks = osm_pkey_tbl_get_num_blocks(p_pkey_tbl);
	max_num_of_blocks = pkey_mgr_get_physp_max_blocks(p_physp);
	if (p_pkey_tbl->max_blocks > max_num_of_blocks) {
		OSM_LOG(p_log, OSM_LOG_INFO,
			"Max number of blocks reduced from %u to %u "
			"for node 0x%016" PRIx64 " port %u (%s)\n",
			p_pkey_tbl->max_blocks, max_num_of_blocks,
			cl_ntoh64(osm_node_get_node_guid(p_node)),
			osm_physp_get_port_num(p_physp),
			p_physp->p_node->print_desc);
	}
	p_pkey_tbl->max_blocks = max_num_of_blocks;

	osm_pkey_tbl_init_new_blocks(p_pkey_tbl);
	p_pkey_tbl->used_blocks = 0;

	/*
	   process every pending pkey in order -
	   first must be "updated" last are "new"
	 */
	p_pending =
	    (osm_pending_pkey_t *) cl_qlist_remove_head(&p_pkey_tbl->pending);
	while (p_pending !=
	       (osm_pending_pkey_t *) cl_qlist_end(&p_pkey_tbl->pending)) {

		found = FALSE;
		ptr = NULL;

		if (p_pending->is_new == FALSE) {
			block_index = p_pending->block;
			pkey_index = p_pending->index;
			found = TRUE;
		} else {
			ptr = cl_map_get(&p_pkey_tbl->accum_pkeys,p_pending->pkey);
			if (ptr != NULL) {
				pkey_idx_ptr = (uintptr_t) ptr;
				pkey_idx = pkey_idx_ptr;
				pkey_idx--; /* adjust pkey index for bias */
				block_index = pkey_idx / IB_NUM_PKEY_ELEMENTS_IN_BLOCK;
				pkey_index = pkey_idx % IB_NUM_PKEY_ELEMENTS_IN_BLOCK;
				found = TRUE;
			}

			if (!found) {
				if (last_accum_pkey_index(p_pkey_tbl,
						      &last_free_block_index,
						      &last_free_pkey_index)) {
					block_index = last_free_block_index;
					pkey_index = last_free_pkey_index + 1;
				} else {
					block_index = 0;
					pkey_index = 0;
				}
				if (pkey_index >= IB_NUM_PKEY_ELEMENTS_IN_BLOCK) {
					block_index++;
					pkey_index -= IB_NUM_PKEY_ELEMENTS_IN_BLOCK;
				}
				if (block_index * IB_NUM_PKEY_ELEMENTS_IN_BLOCK + pkey_index >= pkey_mgr_get_physp_max_pkeys(p_physp)) {
					last_free_block_index = 0;
					last_free_pkey_index = 0;
					found = osm_pkey_find_next_free_entry(p_pkey_tbl, &last_free_block_index, &last_free_pkey_index);
					if (!found)
						full = 1;
					else {
						block_index = last_free_block_index;
						pkey_index = last_free_pkey_index;
						if (block_index * IB_NUM_PKEY_ELEMENTS_IN_BLOCK + pkey_index >= pkey_mgr_get_physp_max_pkeys(p_physp)) {
							full = 1;
							found = FALSE;
						} else {
							OSM_LOG(p_log, OSM_LOG_INFO,
								"Reusing PKeyTable block index %u pkey index %u "
								"for pkey 0x%x on 0x%016" PRIx64 " port %u (%s)\n",
								block_index,
								pkey_index,
								cl_ntoh16(p_pending->pkey),
								cl_ntoh64(osm_node_get_node_guid(p_node)),
								osm_physp_get_port_num(p_physp),
								p_physp->p_node->print_desc);

							clear_accum_pkey_index(p_pkey_tbl, block_index * IB_NUM_PKEY_ELEMENTS_IN_BLOCK + pkey_index);
						}
					}
					if (full)
						OSM_LOG(p_log, OSM_LOG_ERROR,
							"ERR 0512: "
							"Failed to set PKey 0x%04x because Pkey table is full "
							"for node 0x%016" PRIx64 " port %u (%s)\n",
							cl_ntoh16(p_pending->pkey),
							cl_ntoh64(osm_node_get_node_guid(p_node)),
							osm_physp_get_port_num(p_physp),
							p_physp->p_node->print_desc);
				} else
					found = TRUE;
			}
		}

		if (found) {
			if (IB_SUCCESS !=
			    osm_pkey_tbl_set_new_entry(p_pkey_tbl, block_index,
						       pkey_index,
						       p_pending->pkey)) {
				OSM_LOG(p_log, OSM_LOG_ERROR, "ERR 0505: "
					"Failed to set PKey 0x%04x in block %u idx %u "
					"for node 0x%016" PRIx64 " port %u (%s)\n",
					cl_ntoh16(p_pending->pkey), block_index,
					pkey_index,
					cl_ntoh64(osm_node_get_node_guid
						  (p_node)),
					osm_physp_get_port_num(p_physp),
					p_physp->p_node->print_desc);
			}
			if (ptr == NULL &&
			    CL_SUCCESS !=
			    osm_pkey_tbl_set_accum_pkeys(p_pkey_tbl,
							 p_pending->pkey,
							 block_index * IB_NUM_PKEY_ELEMENTS_IN_BLOCK + pkey_index)) {
				OSM_LOG(p_log, OSM_LOG_ERROR, "ERR 0508: "
					"Failed to set accum_pkeys PKey 0x%04x "
					"in block %u idx %u for node 0x%016"
					PRIx64 " port %u (%s)\n",
					cl_ntoh16(p_pending->pkey), block_index,
					pkey_index,
					cl_ntoh64(osm_node_get_node_guid(p_node)),
					osm_physp_get_port_num(p_physp),
					p_physp->p_node->print_desc);
			}
		}

		free(p_pending);
		p_pending =
		    (osm_pending_pkey_t *) cl_qlist_remove_head(&p_pkey_tbl->
								pending);
	}

	/* now look for changes and store */
	for (block_index = 0; block_index < num_of_blocks; block_index++) {
		block = osm_pkey_tbl_block_get(p_pkey_tbl, block_index);
		new_block = osm_pkey_tbl_new_block_get(p_pkey_tbl, block_index);
		if (!new_block)
			new_block = &empty_block;
		if (block && !memcmp(new_block, block, sizeof(*block)))
			continue;

		status =
		    pkey_mgr_update_pkey_entry(sm, p_physp, new_block,
					       block_index);
		if (status == IB_SUCCESS)
			OSM_LOG(p_log, OSM_LOG_DEBUG,
				"Updated pkey table block %u for node 0x%016"
				PRIx64 " port %u (%s)\n", block_index,
				cl_ntoh64(osm_node_get_node_guid(p_node)),
				osm_physp_get_port_num(p_physp),
				p_physp->p_node->print_desc);
		else {
			OSM_LOG(p_log, OSM_LOG_ERROR, "ERR 0506: "
				"pkey_mgr_update_pkey_entry() failed to update "
				"pkey table block %u for node 0x%016" PRIx64
				" port %u (%s)\n", block_index,
				cl_ntoh64(osm_node_get_node_guid(p_node)),
				osm_physp_get_port_num(p_physp),
				p_physp->p_node->print_desc);
			ret = -1;
		}
	}

	return ret;
}