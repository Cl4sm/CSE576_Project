static int char_poly(lash_t *p_lash, int rank, int **matrix, int **poly)
{
	osm_log_t *p_log = &p_lash->p_osm->log;
	int ret = -1;
	int i, j;
	int ***m = NULL;
	int *p = NULL;
	int deg = rank;

	OSM_LOG_ENTER(p_log);

	do {
		if (!matrix)
			break;

		if (!(p = poly_alloc(p_lash, deg)))
			break;

		if (!(m = pm_alloc(p_lash, rank, deg))) {
			free(p);
			p = NULL;
			break;
		}

		for (i = 0; i < rank; i++) {
			for (j = 0; j < rank; j++) {
				m[i][j][0] = matrix[i][j];
			}
			m[i][i][1] = -1;
		}

		if (determinant(p_lash, deg, rank, m, p)) {
			free(p);
			p = NULL;
			break;
		}

		ret = 0;
	} while (0);

	pm_free(m, rank);
	*poly = p;

	OSM_LOG_EXIT(p_log);
	return ret;
}