static void classify_mesh_type(lash_t *p_lash, int sw)
{
	osm_log_t *p_log = &p_lash->p_osm->log;
	int i;
	switch_t *s = p_lash->switches[sw];
	const struct mesh_info *t;

	OSM_LOG_ENTER(p_log);

	if (!s->node->poly)
		goto done;

	for (i = 1; (t = &mesh_info[i])->dimension != -1; i++) {
		if (poly_diff(t->degree, t->poly, s))
			continue;

		s->node->type = i;
		s->node->dimension = t->dimension;
		OSM_LOG_EXIT(p_log);
		return;
	}

done:
	s->node->type = 0;
	OSM_LOG_EXIT(p_log);
	return;
}