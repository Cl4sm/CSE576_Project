void osm_dump_all(osm_opensm_t * osm)
{
	if (OSM_LOG_IS_ACTIVE_V2(&osm->log, OSM_LOG_ROUTING)) {
		/* unicast routes */
		osm_dump_qmap_to_file(osm, "opensm-lid-matrix.dump",
				      &osm->subn.sw_guid_tbl, dump_lid_matrix,
				      osm);
		osm_dump_qmap_to_file(osm, "opensm-lfts.dump",
				      &osm->subn.sw_guid_tbl, dump_ucast_lfts,
				      osm);
		if (OSM_LOG_IS_ACTIVE_V2(&osm->log, OSM_LOG_DEBUG))
			dump_qmap(stdout, &osm->subn.sw_guid_tbl,
				  dump_ucast_path_distribution, osm);

		/* An attempt to get osm_switch_recommend_path to report the
		   same routes that a sweep would assign. */
		if (osm->subn.opt.scatter_ports)
			srandom(osm->subn.opt.scatter_ports);

		osm_dump_qmap_to_file(osm, "opensm.fdbs",
				      &osm->subn.sw_guid_tbl,
				      dump_ucast_routes, osm);
		/* multicast routes */
		osm_dump_qmap_to_file(osm, "opensm.mcfdbs",
				      &osm->subn.sw_guid_tbl,
				      dump_mcast_routes, osm);
		/* SL2VL tables */
		if (osm->subn.opt.qos ||
		    (osm->routing_engine_used &&
		     osm->routing_engine_used->update_sl2vl))
			osm_dump_qmap_to_file(osm, "opensm-sl2vl.dump",
					      &osm->subn.port_guid_tbl,
					      dump_sl2vl_tbl, osm);
	}
	osm_dump_qmap_to_file(osm, "opensm-subnet.lst",
			      &osm->subn.node_guid_tbl, dump_topology_node,
			      osm);
	if (OSM_LOG_IS_ACTIVE_V2(&osm->log, OSM_LOG_VERBOSE))
		print_report(osm, stdout);
}