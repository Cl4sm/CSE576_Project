static struct osm_routing_engine *setup_routing_engine(osm_opensm_t *osm,
						       const char *name)
{
	struct osm_routing_engine *re;
	const struct routing_engine_module *m;

	if (!strcmp(name, "no_fallback")) {
		osm->no_fallback_routing_engine = TRUE;
		return NULL;
	}

	for (m = routing_modules; m->name && *m->name; m++) {
		if (!strcmp(m->name, name)) {
			re = malloc(sizeof(struct osm_routing_engine));
			if (!re) {
				OSM_LOG(&osm->log, OSM_LOG_VERBOSE,
					"memory allocation failed\n");
				return NULL;
			}
			memset(re, 0, sizeof(struct osm_routing_engine));

			re->name = m->name;
			re->type = osm_routing_engine_type(m->name);
			if (m->setup(re, osm)) {
				OSM_LOG(&osm->log, OSM_LOG_VERBOSE,
					"setup of routing"
					" engine \'%s\' failed\n", name);
				free(re);
				return NULL;
			}
			OSM_LOG(&osm->log, OSM_LOG_DEBUG,
				"\'%s\' routing engine set up\n", re->name);
			if (re->type == OSM_ROUTING_ENGINE_TYPE_MINHOP)
				osm->default_routing_engine = re;
			return re;
		}
	}

	OSM_LOG(&osm->log, OSM_LOG_ERROR,
		"cannot find or setup routing engine \'%s\'\n", name);
	return NULL;
}