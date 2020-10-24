static void cache_add_sw_link(osm_ucast_mgr_t * p_mgr, osm_physp_t *p,
			      uint16_t remote_lid_ho, boolean_t is_ca)
{
	cache_switch_t *p_cache_sw;
	uint16_t lid_ho = cl_ntoh16(osm_node_get_base_lid(p->p_node, 0));

	OSM_LOG_ENTER(p_mgr->p_log);

	if (!lid_ho || !remote_lid_ho || !p->port_num)
		goto Exit;

	OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
		"Caching switch port: lid %u [port %u] -> lid %u (%s)\n",
		lid_ho, p->port_num, remote_lid_ho, (is_ca) ? "CA/RTR" : "SW");

	p_cache_sw = cache_get_sw(p_mgr, lid_ho);
	if (!p_cache_sw) {
		p_cache_sw = cache_sw_new(lid_ho, p->p_node->sw->num_ports);
		if (!p_cache_sw) {
			OSM_LOG(p_mgr->p_log, OSM_LOG_ERROR,
				"ERR AD01: Out of memory - cache is invalid\n");
			osm_ucast_cache_invalidate(p_mgr);
			goto Exit;
		}
		cl_qmap_insert(&p_mgr->cache_sw_tbl, lid_ho,
			       &p_cache_sw->map_item);
	}

	if (p->port_num >= p_cache_sw->num_ports) {
		OSM_LOG(p_mgr->p_log, OSM_LOG_ERROR,
			"ERR AD02: Wrong switch? - cache is invalid\n");
		osm_ucast_cache_invalidate(p_mgr);
		goto Exit;
	}

	if (is_ca)
		cache_sw_set_leaf(p_cache_sw);

	if (p_cache_sw->ports[p->port_num].remote_lid_ho == 0) {
		/* cache this link only if it hasn't been already cached */
		p_cache_sw->ports[p->port_num].remote_lid_ho = remote_lid_ho;
		p_cache_sw->ports[p->port_num].is_leaf = is_ca;
	}
Exit:
	OSM_LOG_EXIT(p_mgr->p_log);
}