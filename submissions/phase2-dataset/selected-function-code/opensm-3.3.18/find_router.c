static ib_net64_t find_router(const osm_sa_t *sa, ib_net64_t prefix)
{
	osm_prefix_route_t *route = NULL;
	osm_router_t *rtr;
	cl_qlist_t *l = &sa->p_subn->prefix_routes_list;
	cl_list_item_t *i;

	OSM_LOG(sa->p_log, OSM_LOG_VERBOSE, "Non local DGID subnet prefix "
		"0x%016" PRIx64 "\n", cl_ntoh64(prefix));

	for (i = cl_qlist_head(l); i != cl_qlist_end(l); i = cl_qlist_next(i)) {
		osm_prefix_route_t *r = (osm_prefix_route_t *)i;
		if (!r->prefix || r->prefix == prefix) {
			route = r;
			break;
		}
	}
	if (!route)
		return 0;

	if (route->guid == 0) /* first router */
		rtr = (osm_router_t *) cl_qmap_head(&sa->p_subn->rtr_guid_tbl);
	else
		rtr = (osm_router_t *) cl_qmap_get(&sa->p_subn->rtr_guid_tbl,
						   route->guid);

	if (rtr == (osm_router_t *) cl_qmap_end(&sa->p_subn->rtr_guid_tbl))
		return 0;

	return osm_port_get_guid(osm_router_get_port_ptr(rtr));
}