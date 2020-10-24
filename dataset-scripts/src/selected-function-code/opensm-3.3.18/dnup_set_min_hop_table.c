static int dnup_set_min_hop_table(IN dnup_t * p_dnup)
{
	osm_subn_t *p_subn = &p_dnup->p_osm->subn;
	osm_log_t *p_log = &p_dnup->p_osm->log;
	osm_switch_t *p_sw;
	struct dnup_node *u;
	cl_map_item_t *item;
	uint8_t max_hops = 0;

	OSM_LOG_ENTER(p_log);

	/* Go over all the switches in the subnet - for each init their Min Hop
	   Table */
	OSM_LOG(p_log, OSM_LOG_VERBOSE,
		"Init Min Hop Table of all switches [\n");

	for (item = cl_qmap_head(&p_dnup->p_osm->subn.sw_guid_tbl);
	     item != cl_qmap_end(&p_dnup->p_osm->subn.sw_guid_tbl);
	     item = cl_qmap_next(item)) {
		p_sw = (osm_switch_t *)item;
		/* Clear Min Hop Table */
		osm_switch_clear_hops(p_sw);
	}

	OSM_LOG(p_log, OSM_LOG_VERBOSE,
		"Init Min Hop Table of all switches ]\n");

	/* Now do the BFS for each port  in the subnet */
	OSM_LOG(p_log, OSM_LOG_VERBOSE,
		"BFS through all port guids in the subnet [\n");

	for (item = cl_qmap_head(&p_dnup->p_osm->subn.sw_guid_tbl);
	     item != cl_qmap_end(&p_dnup->p_osm->subn.sw_guid_tbl);
	     item = cl_qmap_next(item)) {
		p_sw = (osm_switch_t *)item;
		dnup_bfs_by_node(p_log, p_subn, p_sw, 0, &max_hops);
	}
	if(p_subn->opt.connect_roots) {
		/*This is probably not necessary, by I am more comfortable
		 * clearing any possible side effects from the previous
		 * dnup routing pass
		 */
		for (item = cl_qmap_head(&p_dnup->p_osm->subn.sw_guid_tbl);
		     item != cl_qmap_end(&p_dnup->p_osm->subn.sw_guid_tbl);
		     item = cl_qmap_next(item)) {
			p_sw = (osm_switch_t *)item;
			osm_switch_clear_hops(p_sw);
			u = (struct dnup_node *) p_sw->priv;
			u->visited = 0;
		}
		for (item = cl_qmap_head(&p_dnup->p_osm->subn.sw_guid_tbl);
		     item != cl_qmap_end(&p_dnup->p_osm->subn.sw_guid_tbl);
		     item = cl_qmap_next(item)) {
			p_sw = (osm_switch_t *)item;
			dnup_bfs_by_node(p_log, p_subn, p_sw, max_hops + 1, NULL);
		}
	}

	OSM_LOG(p_log, OSM_LOG_VERBOSE,
		"BFS through all port guids in the subnet ]\n");
	/* Cleanup */
	OSM_LOG_EXIT(p_log);
	return 0;
}