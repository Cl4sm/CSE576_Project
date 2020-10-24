static
bool build_torus(struct fabric *f, struct torus *t)
{
	int i, j, k;
	int im1, jm1, km1;
	int ip1, jp1, kp1;
	unsigned nlink;
	struct coord_dirs *o;
	struct f_switch *fsw0, *fsw1;
	struct t_switch ****sw = t->sw;
	bool success = true;

	t->link_pool_sz = f->link_cnt;
	t->link_pool = calloc(1, t->link_pool_sz * sizeof(*t->link_pool));
	if (!t->link_pool) {
		OSM_LOG(&t->osm->log, OSM_LOG_ERROR,
			"ERR 4E31: Allocating torus link pool: %s\n",
			strerror(errno));
		goto out;
	}
	t->fabric = f;

	/*
	 * Get things started by locating the up to seven switches that
	 * define the torus "seed", coordinate directions, and datelines.
	 */
	o = &t->seed[t->seed_idx];

	i = canonicalize(-o->x_dateline, t->x_sz);
	j = canonicalize(-o->y_dateline, t->y_sz);
	k = canonicalize(-o->z_dateline, t->z_sz);

	if (o->xp_link.end[0].port < 0) {
		ip1 = canonicalize(1 - o->x_dateline, t->x_sz);
		fsw0 = find_f_sw(f, o->xp_link.end[0].n_id);
		fsw1 = find_f_sw(f, o->xp_link.end[1].n_id);
		success =
			install_tswitch(t, i, j, k, fsw0) &&
			install_tswitch(t, ip1, j, k, fsw1) && success;
	}
	if (o->xm_link.end[0].port < 0) {
		im1 = canonicalize(-1 - o->x_dateline, t->x_sz);
		fsw0 = find_f_sw(f, o->xm_link.end[0].n_id);
		fsw1 = find_f_sw(f, o->xm_link.end[1].n_id);
		success =
			install_tswitch(t, i, j, k, fsw0) &&
			install_tswitch(t, im1, j, k, fsw1) && success;
	}
	if (o->yp_link.end[0].port < 0) {
		jp1 = canonicalize(1 - o->y_dateline, t->y_sz);
		fsw0 = find_f_sw(f, o->yp_link.end[0].n_id);
		fsw1 = find_f_sw(f, o->yp_link.end[1].n_id);
		success =
			install_tswitch(t, i, j, k, fsw0) &&
			install_tswitch(t, i, jp1, k, fsw1) && success;
	}
	if (o->ym_link.end[0].port < 0) {
		jm1 = canonicalize(-1 - o->y_dateline, t->y_sz);
		fsw0 = find_f_sw(f, o->ym_link.end[0].n_id);
		fsw1 = find_f_sw(f, o->ym_link.end[1].n_id);
		success =
			install_tswitch(t, i, j, k, fsw0) &&
			install_tswitch(t, i, jm1, k, fsw1) && success;
	}
	if (o->zp_link.end[0].port < 0) {
		kp1 = canonicalize(1 - o->z_dateline, t->z_sz);
		fsw0 = find_f_sw(f, o->zp_link.end[0].n_id);
		fsw1 = find_f_sw(f, o->zp_link.end[1].n_id);
		success =
			install_tswitch(t, i, j, k, fsw0) &&
			install_tswitch(t, i, j, kp1, fsw1) && success;
	}
	if (o->zm_link.end[0].port < 0) {
		km1 = canonicalize(-1 - o->z_dateline, t->z_sz);
		fsw0 = find_f_sw(f, o->zm_link.end[0].n_id);
		fsw1 = find_f_sw(f, o->zm_link.end[1].n_id);
		success =
			install_tswitch(t, i, j, k, fsw0) &&
			install_tswitch(t, i, j, km1, fsw1) && success;
	}
	if (!success)
		goto out;

	if (!t->seed_idx)
		OSM_LOG(&t->osm->log, OSM_LOG_INFO,
			"Using torus seed configured as default "
			"(seed sw %d,%d,%d GUID 0x%04"PRIx64").\n",
			i, j, k, cl_ntoh64(sw[i][j][k]->n_id));
	else
		OSM_LOG(&t->osm->log, OSM_LOG_INFO,
			"Using torus seed configured as backup #%u "
			"(seed sw %d,%d,%d GUID 0x%04"PRIx64").\n",
			t->seed_idx, i, j, k, cl_ntoh64(sw[i][j][k]->n_id));

	/*
	 * Search the fabric and construct the expected torus topology.
	 *
	 * The algorithm is to consider the "cube" formed by eight switch
	 * locations bounded by the corners i, j, k and i+1, j+1, k+1.
	 * For each such cube look at the topology of the switches already
	 * placed in the torus, and deduce which new switches can be placed
	 * into their proper locations in the torus.  Examine each cube
	 * multiple times, until the number of links moved into the torus
	 * topology does not change.
	 */
again:
	nlink = t->link_cnt;

	for (k = 0; k < (int)t->z_sz; k++)
		for (j = 0; j < (int)t->y_sz; j++)
			for (i = 0; i < (int)t->x_sz; i++)
				locate_sw(t, i, j, k);

	if (t->link_cnt != nlink)
		goto again;

	/*
	 * Move all other endpoints into torus/mesh.
	 */
	for (k = 0; k < (int)t->z_sz; k++)
		for (j = 0; j < (int)t->y_sz; j++)
			for (i = 0; i < (int)t->x_sz; i++)
				if (!link_srcsink(t, i, j, k)) {
					success = false;
					goto out;
				}
out:
	return success;
}