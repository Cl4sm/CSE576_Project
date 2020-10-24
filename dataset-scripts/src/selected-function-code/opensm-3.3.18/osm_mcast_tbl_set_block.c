ib_api_status_t osm_mcast_tbl_set_block(IN osm_mcast_tbl_t * p_tbl,
					IN const ib_net16_t * p_block,
					IN int16_t block_num,
					IN uint8_t position)
{
	uint32_t i;
	uint16_t mlid_start_ho;

	CL_ASSERT(p_tbl);
	CL_ASSERT(p_block);

	if (block_num > p_tbl->max_block)
		return IB_INVALID_PARAMETER;

	if (position > p_tbl->max_position)
		return IB_INVALID_PARAMETER;

	mlid_start_ho = (uint16_t) (block_num * IB_MCAST_BLOCK_SIZE);

	if (mlid_start_ho + IB_MCAST_BLOCK_SIZE - 1 > p_tbl->mft_depth)
		return IB_INVALID_PARAMETER;

	for (i = 0; i < IB_MCAST_BLOCK_SIZE; i++)
		(*p_tbl->p_mask_tbl)[mlid_start_ho + i][position] = p_block[i];

	if (block_num > p_tbl->max_block_in_use)
		p_tbl->max_block_in_use = (uint16_t) block_num;

	return IB_SUCCESS;
}