static int get_local_geometry(lash_t *p_lash, mesh_t *mesh)
{
	osm_log_t *p_log = &p_lash->p_osm->log;
	int sw;
	int status = 0;

	OSM_LOG_ENTER(p_log);

	for (sw = 0; sw < p_lash->num_switches; sw++) {
		/*
		 * skip switches with more links than MAX_DEGREE
		 * since they will never match a known case
		 */
		if (p_lash->switches[sw]->node->num_links > MAX_DEGREE)
			continue;

		if (get_switch_metric(p_lash, sw)) {
			status = -1;
			goto Exit;
		}
		classify_mesh_type(p_lash, sw);
	}

	remove_edges(p_lash);

	for (sw = 0; sw < p_lash->num_switches; sw++) {
		if (p_lash->switches[sw]->node->type < 0)
			continue;
		classify_switch(p_lash, mesh, sw);
	}

Exit:
	OSM_LOG_EXIT(p_log);
	return status;
}