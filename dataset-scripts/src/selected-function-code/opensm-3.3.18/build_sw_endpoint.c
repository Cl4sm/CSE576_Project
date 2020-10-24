static
bool build_sw_endpoint(struct fabric *f, osm_port_t *osm_port)
{
	int sw_port;
	guid_t sw_guid;
	struct osm_switch *osm_sw;
	struct f_switch *sw;
	struct endpoint *ep;
	bool success = false;

	sw_port = osm_physp_get_port_num(osm_port->p_physp);
	sw_guid = osm_node_get_node_guid(osm_port->p_node);
	osm_sw = osm_port->p_node->sw;

	/*
	 * The switch must already exist.
	 */
	sw = find_f_sw(f, sw_guid);
	if (!sw) {
		OSM_LOG(&f->osm->log, OSM_LOG_ERROR,
			"ERR 4E06: missing switch w/GUID 0x%04"PRIx64"\n",
			cl_ntoh64(sw_guid));
		goto out;
	}
	/*
	 * The endpoint may already exist.
	 */
	if (sw->port[sw_port]) {
		if (sw->port[sw_port]->n_id == sw_guid) {
			ep = sw->port[sw_port];
			goto success;
		} else
			OSM_LOG(&f->osm->log, OSM_LOG_ERROR,
				"ERR 4E07: switch port %d has id "
				"0x%04"PRIx64", expected 0x%04"PRIx64"\n",
				sw_port, cl_ntoh64(sw->port[sw_port]->n_id),
				cl_ntoh64(sw_guid));
		goto out;
	}
	ep = calloc(1, sizeof(*ep));
	if (!ep) {
		OSM_LOG(&f->osm->log, OSM_LOG_ERROR,
			"ERR 4E08: allocating endpoint: %s\n", strerror(errno));
		goto out;
	}
	ep->type = SRCSINK;
	ep->port = sw_port;
	ep->n_id = sw_guid;
	ep->link = NULL;
	ep->sw = sw;

	sw->port[sw_port] = ep;

success:
	/*
	 * Fabric objects are temporary, so don't set osm_sw/osm_port priv
	 * pointers using them.  Wait until torus objects get constructed.
	 */
	sw->osm_switch = osm_sw;
	ep->osm_port = osm_port;

	success = true;
out:
	return success;
}