int osm_do_mesh_analysis(lash_t *p_lash)
{
	osm_log_t *p_log = &p_lash->p_osm->log;
	mesh_t *mesh;
	int max_class_num = 0;
	int max_class_type = -1;
	int i;
	switch_t *s;
	char buf[256], *p;

	OSM_LOG_ENTER(p_log);

	mesh = mesh_create(p_lash);
	if (!mesh)
		goto err;

	if (get_local_geometry(p_lash, mesh))
		goto err;

	if (mesh->num_class == 0) {
		OSM_LOG(p_log, OSM_LOG_INFO,
			"found no likely mesh nodes - done\n");
		goto done;
	}

	/*
	 * find dominant switch class
	 */
	OSM_LOG(p_log, OSM_LOG_INFO, "found %d node class%s\n",
		mesh->num_class, (mesh->num_class == 1) ? "" : "es");
	for (i = 0; i < mesh->num_class; i++) {
		OSM_LOG(p_log, OSM_LOG_INFO,
			"class[%d] has %d members with type = %d\n",
			i, mesh->class_count[i],
			p_lash->switches[mesh->class_type[i]]->node->type);
		if (mesh->class_count[i] > max_class_num) {
			max_class_num = mesh->class_count[i];
			max_class_type = mesh->class_type[i];
		}
	}

	s = p_lash->switches[max_class_type];

	p = buf;
	p += sprintf(p, "%snode shape is ",
		    (mesh->num_class == 1) ? "" : "most common ");

	if (s->node->type) {
		const struct mesh_info *t = &mesh_info[s->node->type];

		for (i = 0; i < t->dimension; i++) {
			p += sprintf(p, "%s%d%s", i? " x " : "", t->size[i],
				(t->size[i] == 6)? "+" : "");
		}
		p += sprintf(p, " mesh\n");

		mesh->dimension = t->dimension;
	} else {
		p += sprintf(p, "unknown geometry\n");
	}

	OSM_LOG(p_log, OSM_LOG_INFO, "%s", buf);

	OSM_LOG(p_log, OSM_LOG_INFO, "poly = %s\n",
		poly_print(s->node->num_links, s->node->poly));

	if (s->node->type) {
		make_geometry(p_lash, max_class_type);

		if (make_coord(p_lash, mesh, max_class_type))
			goto err;

		if (measure_geometry(p_lash, mesh))
			goto err;

		if (reorder_links(p_lash, mesh))
			goto err;

		sort_switches(p_lash, mesh);

		p = buf;
		p += sprintf(p, "found ");
		for (i = 0; i < mesh->dimension; i++)
			p += sprintf(p, "%s%d", i? " x " : "", mesh->size[i]);
		p += sprintf(p, " mesh\n");

		OSM_LOG(p_log, OSM_LOG_INFO, "%s", buf);
	}

	if (OSM_LOG_IS_ACTIVE_V2(p_log, OSM_LOG_DEBUG))
		dump_mesh(p_lash);

done:
	mesh_delete(mesh);
	OSM_LOG_EXIT(p_log);
	return 0;

err:
	mesh_delete(mesh);
	OSM_LOG_EXIT(p_log);
	return -1;
}