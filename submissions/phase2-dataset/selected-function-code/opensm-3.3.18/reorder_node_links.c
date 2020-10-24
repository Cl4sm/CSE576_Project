static int reorder_node_links(lash_t *p_lash, mesh_t *mesh, int sw)
{
	osm_log_t *p_log = &p_lash->p_osm->log;
	switch_t *s = p_lash->switches[sw];
	mesh_node_t *node = s->node;
	int n = node->num_links;
	link_t **links;
	int *axes;
	int i, j, k, l;
	int c;
	int next = 0;
	int dimension = mesh->dimension;

	if (!(links = calloc(n, sizeof(link_t *)))) {
		OSM_LOG(p_log, OSM_LOG_ERROR,
			"Failed allocating links array - out of memory\n");
		return -1;
	}

	if (!(axes = calloc(n, sizeof(int)))) {
		free(links);
		OSM_LOG(p_log, OSM_LOG_ERROR,
			"Failed allocating axes array - out of memory\n");
		return -1;
	}

	/*
	 * find the links with axes
	 */
	for (i = 0; i < dimension; i++) {
		j = mesh->dim_order[i];
		for (k = 1; k <= 2; k++) {
			c = 2*j + k;

			if (node->coord[j] > 0)
				c = opposite(s, c);

			for (l = 0; l < n; l++) {
				if (!node->links[l])
					continue;
				if (node->axes[l] == c) {
					links[next] = node->links[l];
					axes[next] = node->axes[l];
					node->links[l] = NULL;
					next++;
				}
			}
		}
	}

	/*
	 * get the rest
	 */
	for (i = 0; i < n; i++) {
		if (!node->links[i])
			continue;

		links[next] = node->links[i];
		axes[next] = node->axes[i];
		node->links[i] = NULL;
		next++;
	}

	for (i = 0; i < n; i++) {
		node->links[i] = links[i];
		node->axes[i] = axes[i];
	}

	free(links);
	free(axes);

	return 0;
}