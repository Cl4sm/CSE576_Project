boolean_t osm_mcast_tbl_get_block(IN osm_mcast_tbl_t * p_tbl,
				  IN int16_t block_num, IN uint8_t position,
				  OUT ib_net16_t * p_block)
{
	uint32_t i;
	uint16_t mlid_start_ho;

	CL_ASSERT(p_tbl);
	CL_ASSERT(p_block);
	CL_ASSERT(block_num * IB_MCAST_BLOCK_SIZE <= p_tbl->mft_depth);

	if (block_num > p_tbl->max_block_in_use)
		return FALSE;

	if (position > p_tbl->max_position) {
		/*
		   Caller shouldn't do this for efficiency's sake...
		 */
		memset(p_block, 0, IB_SMP_DATA_SIZE);
		return TRUE;
	}

	mlid_start_ho = (uint16_t) (block_num * IB_MCAST_BLOCK_SIZE);

	for (i = 0; i < IB_MCAST_BLOCK_SIZE; i++)
		p_block[i] = (*p_tbl->p_mask_tbl)[mlid_start_ho + i][position];

	return TRUE;
}