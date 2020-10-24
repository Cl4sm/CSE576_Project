boolean_t osm_pkey_find_next_free_entry(IN osm_pkey_tbl_t * p_pkey_tbl,
					OUT uint16_t * p_block_idx,
					OUT uint8_t * p_pkey_idx)
{
	ib_pkey_table_t *p_new_block;

	CL_ASSERT(p_block_idx);
	CL_ASSERT(p_pkey_idx);

	while (*p_block_idx < p_pkey_tbl->max_blocks) {
		if (*p_pkey_idx > IB_NUM_PKEY_ELEMENTS_IN_BLOCK - 1) {
			*p_pkey_idx = 0;
			(*p_block_idx)++;
			if (*p_block_idx >= p_pkey_tbl->max_blocks)
				return FALSE;
		}

		p_new_block =
		    osm_pkey_tbl_new_block_get(p_pkey_tbl, *p_block_idx);

		if (!p_new_block ||
		    ib_pkey_is_invalid(p_new_block->pkey_entry[*p_pkey_idx]))
			return TRUE;
		else
			(*p_pkey_idx)++;
	}
	return FALSE;
}