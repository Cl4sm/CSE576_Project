int osm_mesh_node_create(lash_t *p_lash, switch_t *sw)
{
	osm_log_t *p_log = &p_lash->p_osm->log;
	unsigned i;
	mesh_node_t *node;
	unsigned num_ports = sw->p_sw->num_ports;

	OSM_LOG_ENTER(p_log);

	if (!(node = sw->node = calloc(1, sizeof(mesh_node_t) + num_ports * sizeof(link_t *))))
		goto err;

	for (i = 0; i < num_ports; i++)
		if (!(node->links[i] = calloc(1, sizeof(link_t) + num_ports * sizeof(int))))
			goto err;

	if (!(node->axes = calloc(num_ports, sizeof(int))))
		goto err;

	for (i = 0; i < num_ports; i++) {
		node->links[i]->switch_id = NONE;
	}

	OSM_LOG_EXIT(p_log);
	return 0;

err:
	osm_mesh_node_delete(p_lash, sw);
	OSM_LOG(p_log, OSM_LOG_ERROR,
		"Failed allocating mesh node - out of memory\n");
	OSM_LOG_EXIT(p_log);
	return -1;
}