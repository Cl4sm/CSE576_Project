static void alloc_ports_priv(osm_ucast_mgr_t * mgr)
{
	cl_qmap_t *port_tbl = &mgr->p_subn->port_guid_tbl;
	struct osm_remote_guids_count *r;
	osm_port_t *port;
	cl_map_item_t *item;
	unsigned lmc;

	for (item = cl_qmap_head(port_tbl); item != cl_qmap_end(port_tbl);
	     item = cl_qmap_next(item)) {
		port = (osm_port_t *) item;
		lmc = ib_port_info_get_lmc(&port->p_physp->port_info);
		r = malloc(sizeof(*r) + sizeof(r->guids[0]) * (1 << lmc));
		if (!r) {
			OSM_LOG(mgr->p_log, OSM_LOG_ERROR, "ERR 3A09: "
				"cannot allocate memory to track remote"
				" systems for lmc > 0\n");
			port->priv = NULL;
			continue;
		}
		memset(r, 0, sizeof(*r) + sizeof(r->guids[0]) * (1 << lmc));
		port->priv = r;
	}
}