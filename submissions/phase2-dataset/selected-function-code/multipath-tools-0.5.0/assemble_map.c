assemble_map (struct multipath * mp, char * params, int len)
{
	int i, j;
	int shift, freechar;
	int minio;
	int nr_priority_groups, initial_pg_nr;
	char * p, * f;
	char no_path_retry[] = "queue_if_no_path";
	char retain_hwhandler[] = "retain_attached_hw_handler";
	struct pathgroup * pgp;
	struct path * pp;

	minio = mp->minio;
	p = params;
	freechar = len;

	nr_priority_groups = VECTOR_SIZE(mp->pg);
	initial_pg_nr = (nr_priority_groups ? mp->bestpg : 0);

	f = STRDUP(mp->features);

	/*
	 * We have to set 'queue_if_no_path' here even
	 * to avoid path failures during map reload.
	 */
	if (mp->no_path_retry == NO_PATH_RETRY_UNDEF ||
	    mp->no_path_retry == NO_PATH_RETRY_FAIL) {
		/* remove queue_if_no_path settings */
		condlog(3, "%s: remove queue_if_no_path from '%s'",
			mp->alias, mp->features);
		remove_feature(&f, no_path_retry);
	} else {
		add_feature(&f, no_path_retry);
	}
	if (mp->retain_hwhandler == RETAIN_HWHANDLER_ON)
		add_feature(&f, retain_hwhandler);

	shift = snprintf(p, freechar, "%s %s %i %i",
			 f, mp->hwhandler,
			 nr_priority_groups, initial_pg_nr);

	FREE(f);

	if (shift >= freechar) {
		condlog(0, "%s: params too small", mp->alias);
		return 1;
	}
	p += shift;
	freechar -= shift;

	vector_foreach_slot (mp->pg, pgp, i) {
		pgp = VECTOR_SLOT(mp->pg, i);
		shift = snprintf(p, freechar, " %s %i 1", mp->selector,
				 VECTOR_SIZE(pgp->paths));
		if (shift >= freechar) {
			condlog(0, "%s: params too small", mp->alias);
			return 1;
		}
		p += shift;
		freechar -= shift;

		vector_foreach_slot (pgp->paths, pp, j) {
			int tmp_minio = minio;

			if (mp->rr_weight == RR_WEIGHT_PRIO
			    && pp->priority > 0)
				tmp_minio = minio * pp->priority;
			if (!strlen(pp->dev_t) ) {
				condlog(0, "dev_t not set for '%s'", pp->dev);
				return 1;
			}
			shift = snprintf(p, freechar, " %s %d",
					 pp->dev_t, tmp_minio);
			if (shift >= freechar) {
				condlog(0, "%s: params too small", mp->alias);
				return 1;
			}
			p += shift;
			freechar -= shift;
		}
	}
	if (freechar < 1) {
		condlog(0, "%s: params too small", mp->alias);
		return 1;
	}
	snprintf(p, 1, "\n");

	condlog(3, "%s: assembled map [%s]", mp->alias, params);
	return 0;
}
