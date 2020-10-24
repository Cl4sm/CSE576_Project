static
unsigned tsw_changes(struct t_switch *nsw, struct t_switch *osw)
{
	unsigned p, cnt = 0, port_cnt;
	struct endpoint *npt, *opt;
	struct endpoint *rnpt, *ropt;

	if (nsw && !osw) {
		cnt++;
		OSM_LOG(&nsw->torus->osm->log, OSM_LOG_INFO,
			"New torus switch %d,%d,%d GUID 0x%04"PRIx64"\n",
			nsw->i, nsw->j, nsw->k, cl_ntoh64(nsw->n_id));
		goto out;
	}
	if (osw && !nsw) {
		cnt++;
		OSM_LOG(&osw->torus->osm->log, OSM_LOG_INFO,
			"Lost torus switch %d,%d,%d GUID 0x%04"PRIx64"\n",
			osw->i, osw->j, osw->k, cl_ntoh64(osw->n_id));
		goto out;
	}
	if (!(nsw && osw))
		goto out;

	if (nsw->n_id != osw->n_id) {
		cnt++;
		OSM_LOG(&nsw->torus->osm->log, OSM_LOG_INFO,
			"Torus switch %d,%d,%d GUID "
			"was 0x%04"PRIx64", now 0x%04"PRIx64"\n",
			nsw->i, nsw->j, nsw->k,
			cl_ntoh64(osw->n_id), cl_ntoh64(nsw->n_id));
	}

	if (nsw->port_cnt != osw->port_cnt) {
		cnt++;
		OSM_LOG(&nsw->torus->osm->log, OSM_LOG_INFO,
			"Torus switch %d,%d,%d GUID 0x%04"PRIx64" "
			"had %d ports, now has %d\n",
			nsw->i, nsw->j, nsw->k, cl_ntoh64(nsw->n_id),
			osw->port_cnt, nsw->port_cnt);
	}
	port_cnt = nsw->port_cnt;
	if (port_cnt > osw->port_cnt)
		port_cnt = osw->port_cnt;

	for (p = 0; p < port_cnt; p++) {
		npt = nsw->port[p];
		opt = osw->port[p];

		if (npt && npt->link) {
			if (&npt->link->end[0] == npt)
				rnpt = &npt->link->end[1];
			else
				rnpt = &npt->link->end[0];
		} else
			rnpt = NULL;

		if (opt && opt->link) {
			if (&opt->link->end[0] == opt)
				ropt = &opt->link->end[1];
			else
				ropt = &opt->link->end[0];
		} else
			ropt = NULL;

		if (rnpt && !ropt) {
			++cnt;
			OSM_LOG(&nsw->torus->osm->log, OSM_LOG_INFO,
				"Torus switch %d,%d,%d GUID 0x%04"PRIx64"[%d] "
				"remote now %s GUID 0x%04"PRIx64"[%d], "
				"was missing\n",
				nsw->i, nsw->j, nsw->k, cl_ntoh64(nsw->n_id),
				p, rnpt->type == PASSTHRU ? "sw" : "node",
				cl_ntoh64(rnpt->n_id), rnpt->port);
			continue;
		}
		if (ropt && !rnpt) {
			++cnt;
			OSM_LOG(&nsw->torus->osm->log, OSM_LOG_INFO,
				"Torus switch %d,%d,%d GUID 0x%04"PRIx64"[%d] "
				"remote now missing, "
				"was %s GUID 0x%04"PRIx64"[%d]\n",
				osw->i, osw->j, osw->k, cl_ntoh64(nsw->n_id),
				p, ropt->type == PASSTHRU ? "sw" : "node",
				cl_ntoh64(ropt->n_id), ropt->port);
			continue;
		}
		if (!(rnpt && ropt))
			continue;

		if (rnpt->n_id != ropt->n_id) {
			++cnt;
			OSM_LOG(&nsw->torus->osm->log, OSM_LOG_INFO,
				"Torus switch %d,%d,%d GUID 0x%04"PRIx64"[%d] "
				"remote now %s GUID 0x%04"PRIx64"[%d], "
				"was %s GUID 0x%04"PRIx64"[%d]\n",
				nsw->i, nsw->j, nsw->k, cl_ntoh64(nsw->n_id),
				p, rnpt->type == PASSTHRU ? "sw" : "node",
				cl_ntoh64(rnpt->n_id), rnpt->port,
				ropt->type == PASSTHRU ? "sw" : "node",
				cl_ntoh64(ropt->n_id), ropt->port);
			continue;
		}
	}
out:
	return cnt;
}