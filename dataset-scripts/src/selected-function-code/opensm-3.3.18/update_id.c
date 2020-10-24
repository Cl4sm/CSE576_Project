static int update_id(void *cxt, uint64_t guid, char *p)
{
	osm_opensm_t *osm = cxt;
	osm_switch_t *sw;
	uint64_t id;
	char *e;

	sw = osm_get_switch_by_guid(&osm->subn, cl_hton64(guid));
	if (!sw) {
		OSM_LOG(&osm->log, OSM_LOG_VERBOSE,
			"switch with guid 0x%" PRIx64 " is not found\n", guid);
		return 0;
	}

	id = strtoull(p, &e, 0);
	if (*e && !isspace(*e)) {
		OSM_LOG(&osm->log, OSM_LOG_ERROR,
			"ERR AA05: cannot parse node id \'%s\'", p);
		return -1;
	}

	OSM_LOG(&osm->log, OSM_LOG_DEBUG,
		"update node 0x%" PRIx64 " id to 0x%" PRIx64 "\n", guid, id);

	((struct updn_node *)sw->priv)->id = id;

	return 0;
}