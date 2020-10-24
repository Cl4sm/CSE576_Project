static
bool parse_torus(struct torus *t, const char *parse_sep)
{
	unsigned i, j, k, cnt;
	char *ptr;
	bool success = false;

	/*
	 * There can be only one.  Ignore the imposters.
	 */
	if (t->sw_pool)
		goto out;

	if (!parse_size(&t->x_sz, &t->flags, X_MESH, parse_sep))
		goto out;

	if (!parse_size(&t->y_sz, &t->flags, Y_MESH, parse_sep))
		goto out;

	if (!parse_size(&t->z_sz, &t->flags, Z_MESH, parse_sep))
		goto out;

	/*
	 * Set up a linear array of switch pointers big enough to hold
	 * all expected switches.
	 */
	t->sw_pool_sz = t->x_sz * t->y_sz * t->z_sz;
	t->sw_pool = calloc(t->sw_pool_sz, sizeof(*t->sw_pool));
	if (!t->sw_pool) {
		OSM_LOG(&t->osm->log, OSM_LOG_ERROR,
			"ERR 4E0C: Torus switch array calloc: %s\n",
			strerror(errno));
		goto out;
	}
	/*
	 * Set things up so that t->sw[i][j][k] can point to the i,j,k switch.
	 */
	cnt = t->x_sz * (1 + t->y_sz * (1 + t->z_sz));
	t->sw = malloc(cnt * sizeof(void *));
	if (!t->sw) {
		OSM_LOG(&t->osm->log, OSM_LOG_ERROR,
			"ERR 4E0D: Torus switch array malloc: %s\n",
			strerror(errno));
		goto out;
	}
	ptr = (void *)(t->sw);

	ptr += t->x_sz * sizeof(void *);
	for (i = 0; i < t->x_sz; i++) {
		t->sw[i] = (void *)ptr;
		ptr += t->y_sz * sizeof(void *);
	}
	for (i = 0; i < t->x_sz; i++)
		for (j = 0; j < t->y_sz; j++) {
			t->sw[i][j] = (void *)ptr;
			ptr += t->z_sz * sizeof(void *);
		}

	for (i = 0; i < t->x_sz; i++)
		for (j = 0; j < t->y_sz; j++)
			for (k = 0; k < t->z_sz; k++)
				t->sw[i][j][k] = NULL;

	success = true;
out:
	return success;
}