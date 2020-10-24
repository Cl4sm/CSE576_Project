static void classify_switch(lash_t *p_lash, mesh_t *mesh, int sw)
{
	osm_log_t *p_log = &p_lash->p_osm->log;
	int i;
	switch_t *s = p_lash->switches[sw];
	switch_t *s1;

	OSM_LOG_ENTER(p_log);

	if (!s->node->poly)
		goto done;

	for (i = 0; i < mesh->num_class; i++) {
		s1 = p_lash->switches[mesh->class_type[i]];

		if (poly_diff(s->node->num_links, s->node->poly, s1))
			continue;

		mesh->class_count[i]++;
		goto done;
	}

	mesh->class_type[mesh->num_class] = sw;
	mesh->class_count[mesh->num_class] = 1;
	mesh->num_class++;

done:
	OSM_LOG_EXIT(p_log);
}