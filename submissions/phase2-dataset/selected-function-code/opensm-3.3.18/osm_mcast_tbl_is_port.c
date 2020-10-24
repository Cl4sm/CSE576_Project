boolean_t osm_mcast_tbl_is_port(IN const osm_mcast_tbl_t * p_tbl,
				IN uint16_t mlid_ho, IN uint8_t port_num)
{
	unsigned mlid_offset, mask_offset, bit_mask;

	CL_ASSERT(p_tbl);

	if (p_tbl->p_mask_tbl) {
		CL_ASSERT(port_num <=
			  (p_tbl->max_position + 1) * IB_MCAST_MASK_SIZE);
		CL_ASSERT(mlid_ho >= IB_LID_MCAST_START_HO);
		CL_ASSERT(mlid_ho <= p_tbl->max_mlid_ho);

		mlid_offset = mlid_ho - IB_LID_MCAST_START_HO;
		mask_offset = port_num / IB_MCAST_MASK_SIZE;
		bit_mask = cl_ntoh16((uint16_t)
				     (1 << (port_num % IB_MCAST_MASK_SIZE)));
		return (((*p_tbl->
			  p_mask_tbl)[mlid_offset][mask_offset] & bit_mask) ==
			bit_mask);
	}

	return FALSE;
}