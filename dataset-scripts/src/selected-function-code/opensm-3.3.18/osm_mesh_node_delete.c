void osm_mesh_node_delete(lash_t *p_lash, switch_t *sw)
{
	osm_log_t *p_log = &p_lash->p_osm->log;
	unsigned i;
	mesh_node_t *node = sw->node;
	unsigned num_ports = sw->p_sw->num_ports;

	OSM_LOG_ENTER(p_log);

	if (node) {
		for (i = 0; i < num_ports; i++)
			if (node->links[i])
				free(node->links[i]);

		if (node->poly)
			free(node->poly);

		if (node->matrix) {
			for (i = 0; i < node->num_links; i++) {
				if (node->matrix[i])
					free(node->matrix[i]);
			}
			free(node->matrix);
		}

		if (node->axes)
			free(node->axes);

		if (node->coord)
			free(node->coord);

		free(node);

		sw->node = NULL;
	}

	OSM_LOG_EXIT(p_log);
}