int osm_ucast_dfsssp_setup(struct osm_routing_engine *r, osm_opensm_t * p_osm)
{
	/* create context container and add ucast management object */
	dfsssp_context_t *dfsssp_context =
	    dfsssp_context_create(p_osm, OSM_ROUTING_ENGINE_TYPE_DFSSSP);
	if (!dfsssp_context) {
		return 1;	/* alloc failed -> skip this routing */
	}

	/* reset function pointers to dfsssp routines */
	r->context = (void *)dfsssp_context;
	r->build_lid_matrices = dfsssp_build_graph;
	r->ucast_build_fwd_tables = dfsssp_do_dijkstra_routing;
	r->mcast_build_stree = dfsssp_do_mcast_routing;
	r->path_sl = get_dfsssp_sl;
	r->destroy = delete;

	/* we initialize with the current time to achieve a 'good' randomized
	   assignment in get_dfsssp_sl(...)
	 */
	srand(time(NULL));

	return 0;
}