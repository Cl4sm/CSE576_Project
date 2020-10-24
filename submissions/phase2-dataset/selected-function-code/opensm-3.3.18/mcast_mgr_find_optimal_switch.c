static osm_switch_t *mcast_mgr_find_optimal_switch(osm_sm_t * sm,
						   cl_qlist_t * list)
{
	cl_qmap_t mgrp_sw_map;
	cl_qmap_t *p_sw_tbl;
	osm_switch_t *p_sw, *p_best_sw = NULL;
	float hops = 0;
	float best_hops = 10000;	/* any big # will do */

	OSM_LOG_ENTER(sm->p_log);

	p_sw_tbl = &sm->p_subn->sw_guid_tbl;

	create_mgrp_switch_map(&mgrp_sw_map, list);
	for (p_sw = (osm_switch_t *) cl_qmap_head(p_sw_tbl);
	     p_sw != (osm_switch_t *) cl_qmap_end(p_sw_tbl);
	     p_sw = (osm_switch_t *) cl_qmap_next(&p_sw->map_item)) {
		if (!osm_switch_supports_mcast(p_sw))
			continue;

#ifdef OSM_VENDOR_INTF_ANAFA
		hops = mcast_mgr_compute_avg_hops(sm, &mgrp_sw_map, p_sw);
#else
		hops = mcast_mgr_compute_max_hops(sm, &mgrp_sw_map, p_sw);
#endif

		OSM_LOG(sm->p_log, OSM_LOG_DEBUG,
			"Switch 0x%016" PRIx64 ", hops = %f\n",
			cl_ntoh64(osm_node_get_node_guid(p_sw->p_node)), hops);

		if (hops < best_hops) {
			p_best_sw = p_sw;
			best_hops = hops;
		}
	}

	if (p_best_sw)
		OSM_LOG(sm->p_log, OSM_LOG_VERBOSE,
			"Best switch is 0x%" PRIx64 " (%s), hops = %f\n",
			cl_ntoh64(osm_node_get_node_guid(p_best_sw->p_node)),
			p_best_sw->p_node->print_desc, best_hops);
	else
		OSM_LOG(sm->p_log, OSM_LOG_VERBOSE,
			"No multicast capable switches detected\n");

	destroy_mgrp_switch_map(&mgrp_sw_map);
	OSM_LOG_EXIT(sm->p_log);
	return p_best_sw;
}