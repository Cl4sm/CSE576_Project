static
bool torus_master_stree(struct torus *t)
{
	int i, j, k;
	bool success = false;
	struct t_switch *stree_root = find_stree_root(t);

	if (stree_root)
		build_master_stree_branch(stree_root, 0);
	else
		goto out;

	k = stree_root->k;
	for (i = 0; i < t->x_sz; i++) {
		j = stree_root->j;
		if (t->sw[i][j][k])
			build_master_stree_branch(t->sw[i][j][k], 1);

		for (j = 0; j < t->y_sz; j++)
			if (t->sw[i][j][k])
				build_master_stree_branch(t->sw[i][j][k], 2);
	}
	t->master_stree_root = stree_root;
	/*
	 * At this point we should have a master spanning tree that contains
	 * every present switch, for all fabrics that torus-2QoS can route
	 * without deadlocks.  Make sure this is the case; otherwise warn
	 * and return failure so we get bug reports.
	 */
	success = true;
	for (i = 0; i < t->x_sz; i++)
		for (j = 0; j < t->y_sz; j++)
			for (k = 0; k < t->z_sz; k++) {
				struct t_switch *sw = t->sw[i][j][k];
				if (!sw || sw_in_master_stree(sw))
					continue;

				success = false;
				OSM_LOG(&t->osm->log, OSM_LOG_ERROR,
					"ERR 4E43: sw 0x%04"PRIx64" (%d,%d,%d) not in "
					"torus multicast master spanning tree\n",
					cl_ntoh64(sw->n_id), i, j, k);
			}
out:
	return success;
}