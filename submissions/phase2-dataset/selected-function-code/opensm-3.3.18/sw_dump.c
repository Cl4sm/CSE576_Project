static void sw_dump(IN ftree_fabric_t * p_ftree, IN ftree_sw_t * p_sw)
{
	uint32_t i;

	if (!p_sw)
		return;

	if (!OSM_LOG_IS_ACTIVE_V2(&p_ftree->p_osm->log, OSM_LOG_DEBUG))
		return;

	OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_DEBUG,
		"Switch index: %s, GUID: 0x%016" PRIx64
		", Ports: %u DOWN, %u SIBLINGS, %u UP\n",
		tuple_to_str(p_sw->tuple), sw_get_guid_ho(p_sw),
		p_sw->down_port_groups_num, p_sw->sibling_port_groups_num,
		p_sw->up_port_groups_num);

	for (i = 0; i < p_sw->down_port_groups_num; i++)
		port_group_dump(p_ftree, p_sw->down_port_groups[i],
				FTREE_DIRECTION_DOWN);
	for (i = 0; i < p_sw->sibling_port_groups_num; i++)
		port_group_dump(p_ftree, p_sw->sibling_port_groups[i],
				FTREE_DIRECTION_SAME);
	for (i = 0; i < p_sw->up_port_groups_num; i++)
		port_group_dump(p_ftree, p_sw->up_port_groups[i],
				FTREE_DIRECTION_UP);

}				/* sw_dump() */