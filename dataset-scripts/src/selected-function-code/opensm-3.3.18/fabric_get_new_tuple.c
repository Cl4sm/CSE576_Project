static void fabric_get_new_tuple(IN ftree_fabric_t * p_ftree,
				 OUT ftree_tuple_t new_tuple,
				 IN ftree_tuple_t from_tuple,
				 IN ftree_direction_t direction)
{
	ftree_sw_t *p_sw;
	ftree_tuple_t temp_tuple;
	uint8_t var_index;
	uint8_t i;

	tuple_init(new_tuple);
	memcpy(temp_tuple, from_tuple, FTREE_TUPLE_LEN);

	if (direction == FTREE_DIRECTION_DOWN) {
		temp_tuple[0]++;
		var_index = from_tuple[0] + 1;
	} else {
		temp_tuple[0]--;
		var_index = from_tuple[0];
	}

	for (i = 0; i < 0xFF; i++) {
		temp_tuple[var_index] = i;
		p_sw = fabric_get_sw_by_tuple(p_ftree, temp_tuple);
		if (p_sw == NULL)	/* found free tuple */
			break;
	}

	if (i == 0xFF) {
		/* new tuple not found - there are more than 255 ports in one direction */
		return;
	}
	memcpy(new_tuple, temp_tuple, FTREE_TUPLE_LEN);

}				/* fabric_get_new_tuple() */