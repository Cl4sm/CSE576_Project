static
void report_torus_changes(struct torus *nt, struct torus *ot)
{
	unsigned cnt = 0;
	unsigned i, j, k;
	unsigned x_sz = nt->x_sz;
	unsigned y_sz = nt->y_sz;
	unsigned z_sz = nt->z_sz;
	unsigned max_changes = nt->max_changes;

	if (OSM_LOG_IS_ACTIVE_V2(&nt->osm->log, OSM_LOG_ROUTING))
		dump_torus(nt);

	if (!ot)
		return;

	if (x_sz != ot->x_sz) {
		cnt++;
		OSM_LOG(&nt->osm->log, OSM_LOG_INFO,
			"Torus x radix was %d now %d\n",
			ot->x_sz, nt->x_sz);
		if (x_sz > ot->x_sz)
			x_sz = ot->x_sz;
	}
	if (y_sz != ot->y_sz) {
		cnt++;
		OSM_LOG(&nt->osm->log, OSM_LOG_INFO,
			"Torus y radix was %d now %d\n",
			ot->y_sz, nt->y_sz);
		if (y_sz > ot->y_sz)
			y_sz = ot->y_sz;
	}
	if (z_sz != ot->z_sz) {
		cnt++;
		OSM_LOG(&nt->osm->log, OSM_LOG_INFO,
			"Torus z radix was %d now %d\n",
			ot->z_sz, nt->z_sz);
		if (z_sz > ot->z_sz)
			z_sz = ot->z_sz;
	}

	for (k = 0; k < z_sz; k++)
		for (j = 0; j < y_sz; j++)
			for (i = 0; i < x_sz; i++) {
				cnt += tsw_changes(nt->sw[i][j][k],
						   ot->sw[i][j][k]);
				/*
				 * Booting a big fabric will cause lots of
				 * changes as hosts come up, so don't spew.
				 * We want to log changes to learn more about
				 * bouncing links, etc, so they can be fixed.
				 */
				if (cnt > max_changes) {
					OSM_LOG(&nt->osm->log, OSM_LOG_INFO,
						"Too many torus changes; "
						"stopping reporting early\n");
					return;
				}
			}
}