ib_api_status_t osm_pkey_tbl_set_new_entry(IN osm_pkey_tbl_t * p_pkey_tbl,
					   IN uint16_t block_idx,
					   IN uint8_t pkey_idx,
					   IN uint16_t pkey)
{
	ib_pkey_table_t *p_block;

	if (!(p_block = osm_pkey_tbl_new_block_get(p_pkey_tbl, block_idx))) {
		p_block = (ib_pkey_table_t *) malloc(sizeof(ib_pkey_table_t));
		if (!p_block)
			return IB_ERROR;
		memset(p_block, 0, sizeof(ib_pkey_table_t));
		cl_ptr_vector_set(&p_pkey_tbl->new_blocks, block_idx, p_block);
	}

	p_block->pkey_entry[pkey_idx] = pkey;
	if (p_pkey_tbl->used_blocks <= block_idx)
		p_pkey_tbl->used_blocks = block_idx + 1;

	return IB_SUCCESS;
}