static int last_used_pkey_index(const osm_port_t * const p_port,
				const osm_pkey_tbl_t * p_pkey_tbl,
				uint16_t * p_last_index)
{
	ib_pkey_table_t *last_block;
	uint16_t index, last_index = 0;

	CL_ASSERT(p_last_index);

	last_block = osm_pkey_tbl_new_block_get(p_pkey_tbl,
						p_pkey_tbl->used_blocks - 1);
	if (!last_block)
		return 1;

	if (p_pkey_tbl->used_blocks == p_pkey_tbl->max_blocks)
		last_index = cl_ntoh16(p_port->p_node->node_info.partition_cap) % IB_NUM_PKEY_ELEMENTS_IN_BLOCK;
	if (last_index == 0)
		last_index = IB_NUM_PKEY_ELEMENTS_IN_BLOCK;
	index = last_index;
	do {
		index--;
		if (!ib_pkey_is_invalid(last_block->pkey_entry[index]))
			break;
	} while (index != 0);

	*p_last_index = index;
	return 0;
}