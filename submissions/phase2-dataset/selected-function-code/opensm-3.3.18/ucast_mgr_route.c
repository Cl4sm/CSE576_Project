static int ucast_mgr_route(struct osm_routing_engine *r, osm_opensm_t * osm)
{
	int ret;

	OSM_LOG(&osm->log, OSM_LOG_VERBOSE,
		"building routing with \'%s\' routing algorithm...\n", r->name);

	/* Set the before each lft build to keep the routes in place between sweeps */
	if(osm->subn.opt.scatter_ports)
		srandom(osm->subn.opt.scatter_ports);

	if (!r->build_lid_matrices ||
	    (ret = r->build_lid_matrices(r->context)) > 0)
		ret = osm_ucast_mgr_build_lid_matrices(&osm->sm.ucast_mgr);

	if (ret < 0) {
		OSM_LOG(&osm->log, OSM_LOG_ERROR,
			"%s: cannot build lid matrices.\n", r->name);
		return ret;
	}

	if (!r->ucast_build_fwd_tables ||
	    (ret = r->ucast_build_fwd_tables(r->context)) > 0)
		ret = ucast_mgr_build_lfts(&osm->sm.ucast_mgr);

	if (ret < 0) {
		OSM_LOG(&osm->log, OSM_LOG_ERROR,
			"%s: cannot build fwd tables.\n", r->name);
		return ret;
	}

	osm->routing_engine_used = r;

	osm_ucast_mgr_set_fwd_tables(&osm->sm.ucast_mgr);

	return 0;
}