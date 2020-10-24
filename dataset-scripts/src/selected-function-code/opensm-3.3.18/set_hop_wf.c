static int set_hop_wf(void *ctx, uint64_t guid, char *p)
{
	osm_ucast_mgr_t *m = ctx;
	osm_node_t *node = osm_get_node_by_guid(m->p_subn, cl_hton64(guid));
	osm_physp_t *physp;
	unsigned port, hop_wf;
	char *e;

	if (!node || !node->sw) {
		OSM_LOG(m->p_log, OSM_LOG_DEBUG,
			"switch with guid 0x%016" PRIx64 " is not found\n",
			guid);
		return 0;
	}

	if (!p || !*p || !(port = strtoul(p, &e, 0)) || (p == e) ||
	    port >= node->sw->num_ports) {
		OSM_LOG(m->p_log, OSM_LOG_DEBUG,
			"bad port specified for guid 0x%016" PRIx64 "\n", guid);
		return 0;
	}

	p = e + 1;

	if (!*p || !(hop_wf = strtoul(p, &e, 0)) || p == e || hop_wf >= 0x100) {
		OSM_LOG(m->p_log, OSM_LOG_DEBUG,
			"bad hop weight factor specified for guid 0x%016" PRIx64
			"port %u\n", guid, port);
		return 0;
	}

	physp = osm_node_get_physp_ptr(node, port);
	if (!physp)
		return 0;

	physp->hop_wf = hop_wf;

	return 0;
}