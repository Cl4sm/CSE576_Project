static void seed_axes(lash_t *p_lash, int sw)
{
	osm_log_t *p_log = &p_lash->p_osm->log;
	mesh_node_t *node = p_lash->switches[sw]->node;
	int n = node->num_links;
	int i, j, c;

	OSM_LOG_ENTER(p_log);

	if (!node->matrix || !node->dimension)
		goto done;

	for (c = 1; c <= 2*node->dimension; c++) {
		/*
		 * find the next unassigned axis
		 */
		for (i = 0; i < n; i++) {
			if (!node->axes[i])
				break;
		}

		node->axes[i] = c++;

		/*
		 * find the matching opposite direction
		 */
		for (j = 0; j < n; j++) {
			if (node->axes[j] || j == i)
				continue;

			if (node->matrix[i][j] != 2)
				break;
		}

		if (j != n) {
			node->axes[j] = c;
		}
	}

	if (OSM_LOG_IS_ACTIVE_V2(p_log, OSM_LOG_DEBUG)) {
		char buf[256], *p;

		for (i = 0; i < n; i++) {
			p = buf;
			print_axis(p_lash, p, sw, i);
			OSM_LOG(p_log, OSM_LOG_DEBUG, "%s", buf);
		}
	}

done:
	OSM_LOG_EXIT(p_log);
}