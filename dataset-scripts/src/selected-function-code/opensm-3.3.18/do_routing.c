static int do_routing(IN void *context)
{
	ftree_fabric_t *p_ftree = context;
	int status = 0;

	OSM_LOG_ENTER(&p_ftree->p_osm->log);

	if (!p_ftree->fabric_built) {
		status = -1;
		goto Exit;
	}

	OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_VERBOSE,
		"Starting FatTree routing\n");

	OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_VERBOSE,
		"Filling switch forwarding tables for Compute Nodes\n");
	fabric_route_to_cns(p_ftree);

	OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_VERBOSE,
		"Filling switch forwarding tables for non-CN targets\n");
	fabric_route_to_non_cns(p_ftree);

	OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_VERBOSE,
		"Filling switch forwarding tables for switch-to-switch paths\n");
	fabric_route_to_switches(p_ftree);

	if (p_ftree->p_osm->subn.opt.connect_roots) {
		OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_VERBOSE,
			"Connecting switches that are unreachable within "
			"Up/Down rules\n");
		fabric_route_roots(p_ftree);
	}

	/* for each switch, set its fwd table */
	cl_qmap_apply_func(&p_ftree->sw_tbl, set_sw_fwd_table, (void *)p_ftree);

	/* write out hca ordering file */
	fabric_dump_hca_ordering(p_ftree);

	OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_VERBOSE,
		"FatTree routing is done\n");

Exit:
	OSM_LOG_EXIT(&p_ftree->p_osm->log);
	return status;
}