boolean_t osm_mcast_tbl_is_any_port(IN const osm_mcast_tbl_t * p_tbl,
				    IN uint16_t mlid_ho)
{
	unsigned mlid_offset;
	uint8_t position;
	uint16_t result = 0;

	CL_ASSERT(p_tbl);

	if (p_tbl->p_mask_tbl) {
		CL_ASSERT(mlid_ho >= IB_LID_MCAST_START_HO);
		CL_ASSERT(mlid_ho <= p_tbl->max_mlid_ho);

		mlid_offset = mlid_ho - IB_LID_MCAST_START_HO;

		for (position = 0; position <= p_tbl->max_position; position++)
			result |= (*p_tbl->p_mask_tbl)[mlid_offset][position];
	}

	return (result != 0);
}