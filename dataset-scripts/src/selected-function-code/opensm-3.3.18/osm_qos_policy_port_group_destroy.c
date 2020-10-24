void osm_qos_policy_port_group_destroy(osm_qos_port_group_t * p)
{
	osm_qos_port_t * p_port;
	osm_qos_port_t * p_old_port;

	if (!p)
		return;

	if (p->name)
		free(p->name);
	if (p->use)
		free(p->use);

	p_port = (osm_qos_port_t *) cl_qmap_head(&p->port_map);
	while (p_port != (osm_qos_port_t *) cl_qmap_end(&p->port_map))
	{
		p_old_port = p_port;
		p_port = (osm_qos_port_t *) cl_qmap_next(&p_port->map_item);
		free(p_old_port);
	}
	cl_qmap_remove_all(&p->port_map);

	free(p);
}