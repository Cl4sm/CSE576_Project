static
bool build_ca_link(struct fabric *f,
		   osm_port_t *osm_port_ca, guid_t sw_guid, int sw_port)
{
	int ca_port;
	guid_t ca_guid;
	struct link *l;
	struct f_switch *sw;
	bool success = false;

	ca_port = osm_physp_get_port_num(osm_port_ca->p_physp);
	ca_guid = osm_node_get_node_guid(osm_port_ca->p_node);

	/*
	 * The link may already exist.
	 */
	l = find_f_link(f, sw_guid, sw_port, ca_guid, ca_port);
	if (l) {
		success = true;
		goto out;
	}
	/*
	 * The switch must already exist.
	 */
	sw = find_f_sw(f, sw_guid);
	if (!sw) {
		OSM_LOG(&f->osm->log, OSM_LOG_ERROR,
			"ERR 4E09: missing switch w/GUID 0x%04"PRIx64"\n",
			cl_ntoh64(sw_guid));
		goto out;
	}
	l = alloc_flink(f);
	if (!l)
		goto out;

	l->end[0].type = PASSTHRU;
	l->end[0].port = sw_port;
	l->end[0].n_id = sw_guid;
	l->end[0].sw = sw;
	l->end[0].link = l;

	sw->port[sw_port] = &l->end[0];

	l->end[1].type = SRCSINK;
	l->end[1].port = ca_port;
	l->end[1].n_id = ca_guid;
	l->end[1].sw = NULL;		/* Correct for a CA */
	l->end[1].link = l;

	/*
	 * Fabric objects are temporary, so don't set osm_sw/osm_port priv
	 * pointers using them.  Wait until torus objects get constructed.
	 */
	l->end[1].osm_port = osm_port_ca;

	++f->ca_cnt;
	success = true;
out:
	return success;
}