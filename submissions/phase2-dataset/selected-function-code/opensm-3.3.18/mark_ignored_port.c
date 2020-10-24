static int mark_ignored_port(void *ctx, uint64_t guid, char *p)
{
	osm_ucast_mgr_t *m = ctx;
	osm_node_t *node = osm_get_node_by_guid(m->p_subn, cl_hton64(guid));
	osm_physp_t *physp;
	unsigned port;

	if (!node || !node->sw) {
		OSM_LOG(m->p_log, OSM_LOG_DEBUG,
			"switch with guid 0x%016" PRIx64 " is not found\n",
			guid);
		return 0;
	}

	if (!p || !*p || !(port = strtoul(p, NULL, 0)) ||
	    port >= node->sw->num_ports) {
		OSM_LOG(m->p_log, OSM_LOG_DEBUG,
			"bad port specified for guid 0x%016" PRIx64 "\n", guid);
		return 0;
	}

	physp = osm_node_get_physp_ptr(node, port);
	if (!physp)
		return 0;

	physp->is_prof_ignored = 1;

	return 0;
}