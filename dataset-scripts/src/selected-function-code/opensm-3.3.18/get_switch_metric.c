static int get_switch_metric(lash_t *p_lash, int sw)
{
	osm_log_t *p_log = &p_lash->p_osm->log;
	int ret = -1;
	unsigned int i, j, change;
	int sw1, sw2, sw3;
	switch_t *s = p_lash->switches[sw];
	switch_t *s1, *s2, *s3;
	int **m;
	mesh_node_t *node = s->node;
	unsigned int num_links = node->num_links;

	OSM_LOG_ENTER(p_log);

	do {
		if (!(m = m_alloc(p_lash, num_links)))
			break;

		for (i = 0; i < num_links; i++) {
			sw1 = node->links[i]->switch_id;
			s1 = p_lash->switches[sw1];

			/* make all distances big except s1 to itself */
			for (sw2 = 0; sw2 < p_lash->num_switches; sw2++)
				p_lash->switches[sw2]->node->temp = LARGE;

			s1->node->temp = 0;

			do {
				change = 0;

				for (sw2 = 0; sw2 < p_lash->num_switches; sw2++) {
					s2 = p_lash->switches[sw2];
					if (s2->node->temp == LARGE)
						continue;
					for (j = 0; j < s2->node->num_links; j++) {
						sw3 = s2->node->links[j]->switch_id;
						s3 = p_lash->switches[sw3];

						if (sw3 == sw)
							continue;

						if ((s2->node->temp + 1) < s3->node->temp) {
							s3->node->temp = s2->node->temp + 1;
							change++;
						}
					}
				}
			} while (change);

			for (j = 0; j < num_links; j++) {
				sw2 = node->links[j]->switch_id;
				s2 = p_lash->switches[sw2];
				m[i][j] = s2->node->temp;
			}
		}

		if (char_poly(p_lash, num_links, m, &node->poly)) {
			m_free(m, num_links);
			m = NULL;
			break;
		}

		ret = 0;
	} while (0);

	node->matrix = m;

	OSM_LOG_EXIT(p_log);
	return ret;
}