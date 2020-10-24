static void dump_mesh(lash_t *p_lash)
{
	osm_log_t *p_log = &p_lash->p_osm->log;
	int sw;
	int num_switches = p_lash->num_switches;
	int dimension;
	int i, j, k, n;
	switch_t *s, *s2;
	char buf[256];

	OSM_LOG_ENTER(p_log);

	for (sw = 0; sw < num_switches; sw++) {
		s = p_lash->switches[sw];
		dimension = s->node->dimension;
		n = sprintf(buf, "[");
		for (i = 0; i < dimension; i++) {
			n += snprintf(buf + n, sizeof(buf) - n,
				      "%2d", s->node->coord[i]);
			if (n > sizeof(buf))
				n = sizeof(buf);
			if (i != dimension - 1) {
				n += snprintf(buf + n, sizeof(buf) - n, "%s", ",");
				if (n > sizeof(buf))
					n = sizeof(buf);
			}
		}
		n += snprintf(buf + n, sizeof(buf) - n, "]");
		if (n > sizeof(buf))
			n = sizeof(buf);
		for (j = 0; j < s->node->num_links; j++) {
			s2 = p_lash->switches[s->node->links[j]->switch_id];
			n += snprintf(buf + n, sizeof(buf) - n, " [%d]->[", j);
			if (n > sizeof(buf))
				n = sizeof(buf);
			for (k = 0; k < dimension; k++) {
				n += snprintf(buf + n, sizeof(buf) - n, "%2d",
					      s2->node->coord[k]);
				if (n > sizeof(buf))
					n = sizeof(buf);
				if (k != dimension - 1) {
					n += snprintf(buf + n, sizeof(buf) - n,
						      ",");
					if (n > sizeof(buf))
						n = sizeof(buf);
				}
			}
			n += snprintf(buf + n, sizeof(buf) - n, "]");
			if (n > sizeof(buf))
				n = sizeof(buf);
		}
		OSM_LOG(p_log, OSM_LOG_DEBUG, "%s\n", buf);
	}

	OSM_LOG_EXIT(p_log);
}