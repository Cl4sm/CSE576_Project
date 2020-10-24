disassemble_map (vector pathvec, char * params, struct multipath * mpp)
{
	char * word;
	char * p;
	int i, j, k;
	int num_features = 0;
	int num_hwhandler = 0;
	int num_pg = 0;
	int num_pg_args = 0;
	int num_paths = 0;
	int num_paths_args = 0;
	int def_minio = 0;
	int no_path_retry = NO_PATH_RETRY_UNDEF;
	struct path * pp;
	struct pathgroup * pgp;

	p = params;

	condlog(3, "%s: disassemble map [%s]", mpp->alias, params);

	/*
	 * features
	 */
	p += get_word(p, &mpp->features);

	if (!mpp->features)
		return 1;

	num_features = atoi(mpp->features);
	no_path_retry = mpp->no_path_retry;
	mpp->no_path_retry = NO_PATH_RETRY_UNDEF;

	for (i = 0; i < num_features; i++) {
		p += get_word(p, &word);

		if (!word)
			return 1;

		if (merge_words(&mpp->features, word, 1)) {
			FREE(word);
			return 1;
		}
		if ((mpp->no_path_retry == NO_PATH_RETRY_UNDEF) ||
			(mpp->no_path_retry == NO_PATH_RETRY_FAIL) ||
			(mpp->no_path_retry == NO_PATH_RETRY_QUEUE))
			setup_feature(mpp, word);

		FREE(word);
	}

	/*
	 * Reset no_path_retry.
	 * - if not set from features
	 * - if queue_if_no_path is set from features but
	 *   no_path_retry > 0 is selected.
	 */
	if ((mpp->no_path_retry == NO_PATH_RETRY_UNDEF ||
	     mpp->no_path_retry == NO_PATH_RETRY_QUEUE) &&
	    mpp->no_path_retry != no_path_retry)
		mpp->no_path_retry = no_path_retry;

	/*
	 * hwhandler
	 */
	p += get_word(p, &mpp->hwhandler);

	if (!mpp->hwhandler)
		return 1;

	num_hwhandler = atoi(mpp->hwhandler);

	for (i = 0; i < num_hwhandler; i++) {
		p += get_word(p, &word);

		if (!word)
			return 1;

		if (merge_words(&mpp->hwhandler, word, 1)) {
			FREE(word);
			return 1;
		}
		FREE(word);
	}

	/*
	 * nb of path groups
	 */
	p += get_word(p, &word);

	if (!word)
		return 1;

	num_pg = atoi(word);
	FREE(word);

	if (num_pg > 0) {
		if (!mpp->pg) {
			mpp->pg = vector_alloc();
			if (!mpp->pg)
				return 1;
		}
	} else {
		free_pgvec(mpp->pg, KEEP_PATHS);
		mpp->pg = NULL;
	}

	/*
	 * first pg to try
	 */
	p += get_word(p, &word);

	if (!word)
		goto out;

	mpp->nextpg = atoi(word);
	FREE(word);

	for (i = 0; i < num_pg; i++) {
		/*
		 * selector
		 */

		if (!mpp->selector) {
			p += get_word(p, &mpp->selector);

			if (!mpp->selector)
				goto out;

			/*
			 * selector args
			 */
			p += get_word(p, &word);

			if (!word)
				goto out;

			num_pg_args = atoi(word);

			if (merge_words(&mpp->selector, word, 1)) {
				goto out1;
			}
			FREE(word);
		} else {
			p += get_word(p, NULL);
			p += get_word(p, NULL);
		}

		for (j = 0; j < num_pg_args; j++)
			p += get_word(p, NULL);

		/*
		 * paths
		 */
		pgp = alloc_pathgroup();

		if (!pgp)
			goto out;

		if (store_pathgroup(mpp->pg, pgp))
			goto out;

		p += get_word(p, &word);

		if (!word)
			goto out;

		num_paths = atoi(word);
		FREE(word);

		p += get_word(p, &word);

		if (!word)
			goto out;

		num_paths_args = atoi(word);
		FREE(word);

		for (j = 0; j < num_paths; j++) {
			pp = NULL;
			p += get_word(p, &word);

			if (!word)
				goto out;

			if (pathvec)
				pp = find_path_by_devt(pathvec, word);

			if (!pp) {
				pp = alloc_path();

				if (!pp)
					goto out1;

				strncpy(pp->dev_t, word, BLK_DEV_SIZE);

				/* Only call this in multipath client mode */
				if (!conf->daemon && store_path(pathvec, pp))
					goto out1;
			}
			FREE(word);

			if (store_path(pgp->paths, pp))
				goto out;

			/*
			 * Update wwid for multipaths which are not setup
			 * in the get_dm_mpvec() code path
			 */
			if (!strlen(mpp->wwid))
				strncpy(mpp->wwid, pp->wwid, WWID_SIZE);

			/*
			 * Update wwid for paths which may not have been
			 * active at the time the getuid callout was run
			 */
			else if (!strlen(pp->wwid))
				strncpy(pp->wwid, mpp->wwid, WWID_SIZE);

			pgp->id ^= (long)pp;
			pp->pgindex = i + 1;

			for (k = 0; k < num_paths_args; k++)
				if (k == 0) {
					if (!strncmp(mpp->selector,
						     "round-robin", 11)) {
						p += get_word(p, &word);
						def_minio = atoi(word);

						if (mpp->rr_weight == RR_WEIGHT_PRIO
						    && pp->priority > 0)
							def_minio /= pp->priority;

						FREE(word);
					} else {
						p += get_word(p, NULL);
						def_minio = 0;
					}

					if (def_minio != mpp->minio)
						mpp->minio = def_minio;
				}
				else
					p += get_word(p, NULL);

		}
	}
	return 0;
out1:
	FREE(word);
out:
	free_pgvec(mpp->pg, KEEP_PATHS);
	mpp->pg = NULL;
	return 1;
}
