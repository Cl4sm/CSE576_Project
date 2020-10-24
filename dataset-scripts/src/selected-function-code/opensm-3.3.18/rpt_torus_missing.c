static
void rpt_torus_missing(struct torus *t, int i, int j, int k,
		       struct t_switch *sw, int *missing_z)
{
	uint64_t guid_ho;

	if (!sw) {
		/*
		 * We can have multiple missing switches without deadlock
		 * if and only if they are adajacent in the Z direction.
		 */
		if ((t->switch_cnt + 1) < t->sw_pool_sz) {
			if (t->sw[i][j][canonicalize(k - 1, t->z_sz)] &&
			    t->sw[i][j][canonicalize(k + 1, t->z_sz)])
				t->flags |= MSG_DEADLOCK;
		}
		/*
		 * There can be only one such Z-column of missing switches.
		 */
		if (*missing_z < 0)
			*missing_z = i + j * t->x_sz;
		else if (*missing_z != i + j * t->x_sz)
			t->flags |= MSG_DEADLOCK;

		OSM_LOG(&t->osm->log, OSM_LOG_INFO,
			"Missing torus switch at %d,%d,%d\n", i, j, k);
		return;
	}
	guid_ho = cl_ntoh64(sw->n_id);

	if (!(sw->ptgrp[0].port_cnt || (t->x_sz == 1) ||
	      ((t->flags & X_MESH) && i == 0)))
		OSM_LOG(&t->osm->log, OSM_LOG_INFO,
			"Missing torus -x link on "
			"switch %d,%d,%d GUID 0x%04"PRIx64"\n",
			i, j, k, guid_ho);
	if (!(sw->ptgrp[1].port_cnt || (t->x_sz == 1) ||
	      ((t->flags & X_MESH) && (i + 1) == t->x_sz)))
		OSM_LOG(&t->osm->log, OSM_LOG_INFO,
			"Missing torus +x link on "
			"switch %d,%d,%d GUID 0x%04"PRIx64"\n",
			i, j, k, guid_ho);
	if (!(sw->ptgrp[2].port_cnt || (t->y_sz == 1) ||
	      ((t->flags & Y_MESH) && j == 0)))
		OSM_LOG(&t->osm->log, OSM_LOG_INFO,
			"Missing torus -y link on "
			"switch %d,%d,%d GUID 0x%04"PRIx64"\n",
			i, j, k, guid_ho);
	if (!(sw->ptgrp[3].port_cnt || (t->y_sz == 1) ||
	      ((t->flags & Y_MESH) && (j + 1) == t->y_sz)))
		OSM_LOG(&t->osm->log, OSM_LOG_INFO,
			"Missing torus +y link on "
			"switch %d,%d,%d GUID 0x%04"PRIx64"\n",
			i, j, k, guid_ho);
	if (!(sw->ptgrp[4].port_cnt || (t->z_sz == 1) ||
	      ((t->flags & Z_MESH) && k == 0)))
		OSM_LOG(&t->osm->log, OSM_LOG_INFO,
			"Missing torus -z link on "
			"switch %d,%d,%d GUID 0x%04"PRIx64"\n",
			i, j, k, guid_ho);
	if (!(sw->ptgrp[5].port_cnt || (t->z_sz == 1) ||
	      ((t->flags & Z_MESH) && (k + 1) == t->z_sz)))
		OSM_LOG(&t->osm->log, OSM_LOG_INFO,
			"Missing torus +z link on "
			"switch %d,%d,%d GUID 0x%04"PRIx64"\n",
			i, j, k, guid_ho);
}