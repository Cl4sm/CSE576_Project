static void cache_remove_port(osm_ucast_mgr_t * p_mgr, uint16_t lid_ho,
			      uint8_t port_num, uint16_t remote_lid_ho,
			      boolean_t is_ca)
{
	cache_switch_t *p_cache_sw;

	OSM_LOG_ENTER(p_mgr->p_log);

	if (!p_mgr->cache_valid)
		goto Exit;

	p_cache_sw = cache_get_sw(p_mgr, lid_ho);
	if (!p_cache_sw) {
		OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
			"Found uncached switch/link (lid %u, port %u)\n",
			lid_ho, port_num);
		osm_ucast_cache_invalidate(p_mgr);
		goto Exit;
	}

	if (port_num >= p_cache_sw->num_ports ||
	    !p_cache_sw->ports[port_num].remote_lid_ho) {
		OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
			"Found uncached switch link (lid %u, port %u)\n",
			lid_ho, port_num);
		osm_ucast_cache_invalidate(p_mgr);
		goto Exit;
	}

	if (p_cache_sw->ports[port_num].remote_lid_ho != remote_lid_ho) {
		OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
			"Remote lid change on switch lid %u, port %u "
			"(was %u, now %u)\n", lid_ho, port_num,
			p_cache_sw->ports[port_num].remote_lid_ho,
			remote_lid_ho);
		osm_ucast_cache_invalidate(p_mgr);
		goto Exit;
	}

	if ((p_cache_sw->ports[port_num].is_leaf && !is_ca) ||
	    (!p_cache_sw->ports[port_num].is_leaf && is_ca)) {
		OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
			"Remote node type change on switch lid %u, port %u\n",
			lid_ho, port_num);
		osm_ucast_cache_invalidate(p_mgr);
		goto Exit;
	}

	OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
		"New link from lid %u, port %u to lid %u - "
		"found in cache\n", lid_ho, port_num, remote_lid_ho);

	/* the new link was cached - clean it from the cache */

	p_cache_sw->ports[port_num].remote_lid_ho = 0;
	p_cache_sw->ports[port_num].is_leaf = FALSE;
Exit:
	OSM_LOG_EXIT(p_mgr->p_log);
}				/* cache_remove_port() */