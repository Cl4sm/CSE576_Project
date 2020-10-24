static int make_coord(lash_t *p_lash, mesh_t *mesh, int seed)
{
	osm_log_t *p_log = &p_lash->p_osm->log;
	unsigned int i, j, k;
	int sw;
	switch_t *s, *s1;
	unsigned int change;
	unsigned int dimension = mesh->dimension;
	int num_switches = p_lash->num_switches;
	int assigned_axes = 0, unassigned_axes = 0;

	OSM_LOG_ENTER(p_log);

	for (sw = 0; sw < num_switches; sw++) {
		s = p_lash->switches[sw];

		s->node->coord = calloc(dimension, sizeof(int));
		if (!s->node->coord) {
			OSM_LOG(p_log, OSM_LOG_ERROR,
				"Failed allocating coord - out of memory\n");
			OSM_LOG_EXIT(p_log);
			return -1;
		}

		for (i = 0; i < dimension; i++)
			s->node->coord[i] = (sw == seed) ? 0 : LARGE;

		for (i = 0; i < s->node->num_links; i++)
			if (s->node->axes[i] == 0)
				unassigned_axes++;
			else
				assigned_axes++;
	}

	OSM_LOG(p_log, OSM_LOG_DEBUG, "%d/%d unassigned/assigned axes\n",
		unassigned_axes, assigned_axes);

	do {
		change = 0;

		for (sw = 0; sw < num_switches; sw++) {
			s = p_lash->switches[sw];

			if (s->node->coord[0] == LARGE)
				continue;

			for (j = 0; j < s->node->num_links; j++) {
				if (!s->node->axes[j])
					continue;

				s1 = p_lash->switches[s->node->links[j]->switch_id];

				for (k = 0; k < dimension; k++) {
					int coord = s->node->coord[k];
					unsigned axis = s->node->axes[j] - 1;

					if (k == axis/2)
						coord += (axis & 1)? -1 : +1;

					if (ltmag(coord, s1->node->coord[k])) {
						s1->node->coord[k] = coord;
						change++;
					}
				}
			}
		}
	} while (change);

	OSM_LOG_EXIT(p_log);
	return 0;
}