int osm_mcast_tbl_realloc(IN osm_mcast_tbl_t * p_tbl, IN unsigned mlid_offset)
{
	size_t mft_depth, size;
	uint16_t (*p_mask_tbl)[][IB_MCAST_POSITION_MAX + 1];

	if (mlid_offset < p_tbl->mft_depth)
		goto done;

	/*
	   The number of bytes needed in the mask table is:
	   The (maximum bit mask 'position' + 1) times the
	   number of bytes in each bit mask times the
	   number of MLIDs supported by the table.

	   We must always allocate the array with the maximum position
	   since it is (and must be) defined that way the table structure
	   in order to create a pointer to a two dimensional array.
	 */
	mft_depth = (mlid_offset / IB_MCAST_BLOCK_SIZE + 1) * IB_MCAST_BLOCK_SIZE;
	size = mft_depth * (IB_MCAST_POSITION_MAX + 1) * IB_MCAST_MASK_SIZE / 8;
	p_mask_tbl = realloc(p_tbl->p_mask_tbl, size);
	if (!p_mask_tbl)
		return -1;
	memset((uint8_t *)p_mask_tbl + p_tbl->mft_depth * (IB_MCAST_POSITION_MAX + 1) * IB_MCAST_MASK_SIZE / 8,
	       0,
	       size - p_tbl->mft_depth * (IB_MCAST_POSITION_MAX + 1) * IB_MCAST_MASK_SIZE / 8);
	p_tbl->p_mask_tbl = p_mask_tbl;
	p_tbl->mft_depth = mft_depth;
done:
	p_tbl->max_mlid_ho = mlid_offset + IB_LID_MCAST_START_HO;
	return 0;
}