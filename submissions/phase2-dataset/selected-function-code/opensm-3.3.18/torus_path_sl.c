uint8_t torus_path_sl(void *context, uint8_t path_sl_hint,
		      const ib_net16_t slid, const ib_net16_t dlid)
{
	struct torus_context *ctx = context;
	osm_opensm_t *p_osm = ctx->osm;
	osm_log_t *log = &p_osm->log;
	osm_port_t *osm_sport, *osm_dport;
	struct endpoint *sport, *dport;
	struct t_switch *ssw, *dsw;
	struct torus *t;
	guid_t guid;
	unsigned sl = 0;

	osm_sport = osm_get_port_by_lid(&p_osm->subn, slid);
	if (!osm_sport)
		goto out;

	osm_dport = osm_get_port_by_lid(&p_osm->subn, dlid);
	if (!osm_dport)
		goto out;

	sport = osm_sport->priv;
	if (!(sport && sport->osm_port == osm_sport)) {
		sport = osm_port_relink_endpoint(osm_sport);
		if (!sport) {
			guid = osm_node_get_node_guid(osm_sport->p_node);
			OSM_LOG(log, OSM_LOG_INFO,
				"Note: osm_sport (GUID 0x%04"PRIx64") "
				"not in torus fabric description\n",
				cl_ntoh64(guid));
			goto out;
		}
	}
	dport = osm_dport->priv;
	if (!(dport && dport->osm_port == osm_dport)) {
		dport = osm_port_relink_endpoint(osm_dport);
		if (!dport) {
			guid = osm_node_get_node_guid(osm_dport->p_node);
			OSM_LOG(log, OSM_LOG_INFO,
				"Note: osm_dport (GUID 0x%04"PRIx64") "
				"not in torus fabric description\n",
				cl_ntoh64(guid));
			goto out;
		}
	}
	/*
	 * We're only supposed to be called for CA ports, and maybe
	 * switch management ports.
	 */
	if (sport->type != SRCSINK) {
		guid = osm_node_get_node_guid(osm_sport->p_node);
		OSM_LOG(log, OSM_LOG_INFO,
			"Error: osm_sport (GUID 0x%04"PRIx64") "
			"not a data src/sink port\n", cl_ntoh64(guid));
		goto out;
	}
	if (dport->type != SRCSINK) {
		guid = osm_node_get_node_guid(osm_dport->p_node);
		OSM_LOG(log, OSM_LOG_INFO,
			"Error: osm_dport (GUID 0x%04"PRIx64") "
			"not a data src/sink port\n", cl_ntoh64(guid));
		goto out;
	}
	/*
	 * By definition, a CA port is connected to end[1] of a link, and
	 * the switch port is end[0].  See build_ca_link() and link_srcsink().
	 */
	if (sport->link) {
		ssw = sport->link->end[0].sw;
	} else {
		ssw = sport->sw;
	}
	if (dport->link)
		dsw = dport->link->end[0].sw;
	else
		dsw = dport->sw;

	t = ssw->torus;

	sl  = sl_set_use_loop_vl(use_vl1(ssw->i, dsw->i, t->x_sz), 0);
	sl |= sl_set_use_loop_vl(use_vl1(ssw->j, dsw->j, t->y_sz), 1);
	sl |= sl_set_use_loop_vl(use_vl1(ssw->k, dsw->k, t->z_sz), 2);
	sl |= sl_set_qos(sl_get_qos(path_sl_hint));
out:
	return sl;
}