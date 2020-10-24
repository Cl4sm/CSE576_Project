ib_api_status_t osm_pkey_tbl_get_block_and_idx(IN osm_pkey_tbl_t * p_pkey_tbl,
					       IN uint16_t * p_pkey,
					       OUT uint16_t * p_block_idx,
					       OUT uint8_t * p_pkey_idx)
{
	uint16_t num_of_blocks;
	uint16_t block_index;
	ib_pkey_table_t *block;

	CL_ASSERT(p_block_idx != NULL);
	CL_ASSERT(p_pkey_idx != NULL);

	num_of_blocks = (uint16_t) cl_ptr_vector_get_size(&p_pkey_tbl->blocks);
	for (block_index = 0; block_index < num_of_blocks; block_index++) {
		block = osm_pkey_tbl_block_get(p_pkey_tbl, block_index);
		if ((block->pkey_entry <= p_pkey) &&
		    (p_pkey <
		     block->pkey_entry + IB_NUM_PKEY_ELEMENTS_IN_BLOCK)) {
			*p_block_idx = block_index;
			*p_pkey_idx = (uint8_t) (p_pkey - block->pkey_entry);
			return IB_SUCCESS;
		}
	}
	return IB_NOT_FOUND;
}