static ftree_port_group_t *sw_get_port_group_by_remote_lid(IN ftree_sw_t * p_sw,
							   IN uint16_t
							   remote_base_lid,
							   IN ftree_direction_t
							   direction)
{
	uint32_t i;
	uint32_t size;
	ftree_port_group_t **port_groups;

	if (direction == FTREE_DIRECTION_UP) {
		port_groups = p_sw->up_port_groups;
		size = p_sw->up_port_groups_num;
	} else if (direction == FTREE_DIRECTION_SAME) {
		port_groups = p_sw->sibling_port_groups;
		size = p_sw->sibling_port_groups_num;
	} else {
		port_groups = p_sw->down_port_groups;
		size = p_sw->down_port_groups_num;
	}

	for (i = 0; i < size; i++)
		if (remote_base_lid == port_groups[i]->remote_base_lid)
			return port_groups[i];

	return NULL;
}				/* sw_get_port_group_by_remote_lid() */