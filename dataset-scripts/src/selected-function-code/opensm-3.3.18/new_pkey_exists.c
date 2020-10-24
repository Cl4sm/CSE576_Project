static int new_pkey_exists(osm_pkey_tbl_t * p_pkey_tbl, ib_net16_t pkey)
{
	uint16_t num_blocks;
	uint16_t block_index;
	ib_pkey_table_t *block;
	uint16_t pkey_idx;

	num_blocks = (uint16_t) cl_ptr_vector_get_size(&p_pkey_tbl->new_blocks);
	for (block_index = 0; block_index < num_blocks; block_index++) {
		block = osm_pkey_tbl_new_block_get(p_pkey_tbl, block_index);
		if (!block)
			continue;

		for (pkey_idx = 0; pkey_idx < IB_NUM_PKEY_ELEMENTS_IN_BLOCK;
		     pkey_idx++) {
			if (block->pkey_entry[pkey_idx] == pkey)
				return 1;
		}
	}
	return 0;
}