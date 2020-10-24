static void free_lash_structures(lash_t * p_lash)
{
	unsigned int i, j, k;
	unsigned num_switches = p_lash->num_switches;
	osm_log_t *p_log = &p_lash->p_osm->log;

	OSM_LOG_ENTER(p_log);

	delete_mesh_switches(p_lash);

	/* free cdg_vertex_matrix */
	for (i = 0; i < p_lash->vl_min; i++) {
		for (j = 0; j < num_switches; j++) {
			for (k = 0; k < num_switches; k++)
				if (p_lash->cdg_vertex_matrix[i][j][k])
					free(p_lash->cdg_vertex_matrix[i][j][k]);
			if (p_lash->cdg_vertex_matrix[i][j])
				free(p_lash->cdg_vertex_matrix[i][j]);
		}
		if (p_lash->cdg_vertex_matrix[i])
			free(p_lash->cdg_vertex_matrix[i]);
	}

	if (p_lash->cdg_vertex_matrix)
		free(p_lash->cdg_vertex_matrix);

	/* free virtual_location */
	for (i = 0; i < num_switches; i++) {
		for (j = 0; j < num_switches; j++) {
			if (p_lash->virtual_location[i][j])
				free(p_lash->virtual_location[i][j]);
		}
		if (p_lash->virtual_location[i])
			free(p_lash->virtual_location[i]);
	}
	if (p_lash->virtual_location)
		free(p_lash->virtual_location);

	OSM_LOG_EXIT(p_log);
}