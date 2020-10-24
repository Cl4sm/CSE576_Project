static void sw_add_port(IN ftree_sw_t * p_sw, IN uint8_t port_num,
			IN uint8_t remote_port_num, IN uint16_t base_lid,
			IN uint16_t remote_base_lid, IN ib_net64_t port_guid,
			IN ib_net64_t remote_port_guid,
			IN ib_net64_t remote_node_guid,
			IN uint8_t remote_node_type,
			IN void *p_remote_hca_or_sw,
			IN ftree_direction_t direction)
{
	ftree_port_group_t *p_group =
	    sw_get_port_group_by_remote_lid(p_sw, remote_base_lid, direction);

	if (!p_group) {
		p_group = port_group_create(base_lid, remote_base_lid,
					    port_guid, sw_get_guid_no(p_sw),
					    IB_NODE_TYPE_SWITCH, p_sw,
					    remote_port_guid, remote_node_guid,
					    remote_node_type,
					    p_remote_hca_or_sw, FALSE, FALSE);
		CL_ASSERT(p_group);

		if (direction == FTREE_DIRECTION_UP) {
			p_sw->up_port_groups[p_sw->up_port_groups_num++] =
			    p_group;
		} else if (direction == FTREE_DIRECTION_SAME) {
			p_sw->
			    sibling_port_groups[p_sw->sibling_port_groups_num++]
			    = p_group;
		} else
			p_sw->down_port_groups[p_sw->down_port_groups_num++] =
			    p_group;
	}
	port_group_add_port(p_group, port_num, remote_port_num);

}				/* sw_add_port() */