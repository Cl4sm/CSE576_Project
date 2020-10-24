static int set_hops_on_remote_sw(IN ftree_port_group_t * p_group,
				 IN uint16_t target_lid, IN uint8_t hops,
				 IN boolean_t is_target_sw)
{
	ftree_port_t *p_port;
	uint8_t i, ports_num;
	ftree_sw_t *p_remote_sw = p_group->remote_hca_or_sw.p_sw;

	/* if lid is a switch, we set the min hop table in the osm_switch struct */
	CL_ASSERT(p_group->remote_node_type == IB_NODE_TYPE_SWITCH);
	p_remote_sw->hops[target_lid] = hops;

	/* If target lid is a switch we set the min hop table values
	 * for each port on the associated osm_sw struct */
	if (!is_target_sw)
		return 0;

	ports_num = (uint8_t) cl_ptr_vector_get_size(&p_group->ports);
	for (i = 0; i < ports_num; i++) {
		cl_ptr_vector_at(&p_group->ports, i, (void *)&p_port);
		if (sw_set_hops(p_remote_sw, target_lid,
				p_port->remote_port_num, hops, is_target_sw))
			return -1;
	}
	return 0;
}