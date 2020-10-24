static void port_group_dump(IN ftree_fabric_t * p_ftree,
			    IN ftree_port_group_t * p_group,
			    IN ftree_direction_t direction)
{
	ftree_port_t *p_port;
	uint32_t size;
	uint32_t i;
	char buff[10 * 1024];

	if (!p_group)
		return;

	if (!OSM_LOG_IS_ACTIVE_V2(&p_ftree->p_osm->log, OSM_LOG_DEBUG))
		return;

	size = cl_ptr_vector_get_size(&p_group->ports);
	buff[0] = '\0';

	for (i = 0; i < size; i++) {
		cl_ptr_vector_at(&p_group->ports, i, (void *)&p_port);
		CL_ASSERT(p_port);

		if (i != 0)
			strcat(buff, ", ");
		sprintf(buff + strlen(buff), "%u", p_port->port_num);
	}

	OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_DEBUG,
		"    Port Group of size %u, port(s): %s, direction: %s\n"
		"                  Local <--> Remote GUID (LID):"
		"0x%016" PRIx64 " (0x%04x) <--> 0x%016" PRIx64 " (0x%04x)\n",
		size, buff,
		(direction == FTREE_DIRECTION_DOWN) ? "DOWN" : (direction ==
								FTREE_DIRECTION_SAME)
		? "SIBLING" : "UP", cl_ntoh64(p_group->port_guid),
		p_group->base_lid, cl_ntoh64(p_group->remote_port_guid),
		p_group->remote_base_lid);

}				/* port_group_dump() */