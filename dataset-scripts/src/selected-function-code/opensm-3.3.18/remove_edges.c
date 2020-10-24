static void remove_edges(lash_t *p_lash)
{
	osm_log_t *p_log = &p_lash->p_osm->log;
	int sw;
	mesh_node_t *n, *nn;
	unsigned i;

	OSM_LOG_ENTER(p_log);

	for (sw = 0; sw < p_lash->num_switches; sw++) {
		n = p_lash->switches[sw]->node;
		if (!n->type)
			continue;

		for (i = 0; i < n->num_links; i++) {
			nn = p_lash->switches[n->links[i]->switch_id]->node;

			if (nn->num_links > n->num_links) {
				OSM_LOG(p_log, OSM_LOG_DEBUG,
					"removed edge switch %s\n",
					p_lash->switches[sw]->p_sw->p_node->print_desc);
				n->type = -1;
				break;
			}
		}
	}

	OSM_LOG_EXIT(p_log);
}