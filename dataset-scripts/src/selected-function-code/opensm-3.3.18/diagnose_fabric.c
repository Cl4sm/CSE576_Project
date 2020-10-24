static
void diagnose_fabric(struct fabric *f)
{
	struct link *l;
	struct endpoint *ep;
	unsigned k, p;

	/*
	 * Report on any links that didn't get transferred to the torus.
	 */
	for (k = 0; k < f->link_cnt; k++) {
		l = f->link[k];

		if (!(l->end[0].sw && l->end[1].sw))
			continue;

		OSM_LOG(&f->osm->log, OSM_LOG_INFO,
			"Found non-torus fabric link:"
			" sw GUID 0x%04"PRIx64" port %d <->"
			" sw GUID 0x%04"PRIx64" port %d\n",
			cl_ntoh64(l->end[0].n_id), l->end[0].port,
			cl_ntoh64(l->end[1].n_id), l->end[1].port);
	}
	/*
	 * Report on any switches with ports using endpoints that didn't
	 * get transferred to the torus.
	 */
	for (k = 0; k < f->switch_cnt; k++)
		for (p = 0; p < f->sw[k]->port_cnt; p++) {

			if (!f->sw[k]->port[p])
				continue;

			ep = f->sw[k]->port[p];

			/*
			 * We already reported on inter-switch links above.
			 */
			if (ep->type == PASSTHRU)
				continue;

			OSM_LOG(&f->osm->log, OSM_LOG_INFO,
				"Found non-torus fabric port:"
				" sw GUID 0x%04"PRIx64" port %d\n",
				cl_ntoh64(f->sw[k]->n_id), p);
		}
}