void osm_mcast_tbl_init(IN osm_mcast_tbl_t * p_tbl, IN uint8_t num_ports,
			IN uint16_t capacity)
{
	CL_ASSERT(p_tbl);
	CL_ASSERT(num_ports);

	memset(p_tbl, 0, sizeof(*p_tbl));

	p_tbl->max_block_in_use = -1;

	if (capacity == 0) {
		/*
		   This switch apparently doesn't support multicast.
		   Everything is initialized to zero already, so return.
		 */
		return;
	}

	p_tbl->num_entries = capacity;
	p_tbl->num_ports = num_ports;
	p_tbl->max_position =
	    (uint8_t) ((ROUNDUP(num_ports, IB_MCAST_MASK_SIZE) /
			IB_MCAST_MASK_SIZE) - 1);

	p_tbl->max_block = (uint16_t) ((ROUNDUP(p_tbl->num_entries,
						IB_MCAST_BLOCK_SIZE) /
					IB_MCAST_BLOCK_SIZE) - 1);
}