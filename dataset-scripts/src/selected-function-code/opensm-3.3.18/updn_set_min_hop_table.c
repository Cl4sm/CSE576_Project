static int updn_set_min_hop_table(IN updn_t * p_updn)
{
	osm_subn_t *p_subn = &p_updn->p_osm->subn;
	osm_log_t *p_log = &p_updn->p_osm->log;
	osm_switch_t *p_sw;
	cl_map_item_t *item;

	OSM_LOG_ENTER(p_log);

	/* Go over all the switches in the subnet - for each init their Min Hop
	   Table */
	OSM_LOG(p_log, OSM_LOG_VERBOSE,
		"Init Min Hop Table of all switches [\n");

	for (item = cl_qmap_head(&p_updn->p_osm->subn.sw_guid_tbl);
	     item != cl_qmap_end(&p_updn->p_osm->subn.sw_guid_tbl);
	     item = cl_qmap_next(item)) {
		p_sw = (osm_switch_t *)item;
		/* Clear Min Hop Table */
		if (p_subn->opt.connect_roots)
			updn_clear_non_root_hops(p_updn, p_sw);
		else
			osm_switch_clear_hops(p_sw);
	}

	OSM_LOG(p_log, OSM_LOG_VERBOSE,
		"Init Min Hop Table of all switches ]\n");

	/* Now do the BFS for each port  in the subnet */
	OSM_LOG(p_log, OSM_LOG_VERBOSE,
		"BFS through all port guids in the subnet [\n");

	for (item = cl_qmap_head(&p_updn->p_osm->subn.sw_guid_tbl);
	     item != cl_qmap_end(&p_updn->p_osm->subn.sw_guid_tbl);
	     item = cl_qmap_next(item)) {
		p_sw = (osm_switch_t *)item;
		updn_bfs_by_node(p_log, p_subn, p_sw);
	}

	OSM_LOG(p_log, OSM_LOG_VERBOSE,
		"BFS through all port guids in the subnet ]\n");
	/* Cleanup */
	OSM_LOG_EXIT(p_log);
	return 0;
}