disassemble_status (char * params, struct multipath * mpp)
{
	char * word;
	char * p;
	int i, j, k;
	int num_feature_args;
	int num_hwhandler_args;
	int num_pg;
	int num_pg_args;
	int num_paths;
	int def_minio = 0;
	struct path * pp;
	struct pathgroup * pgp;

	p = params;

	condlog(3, "%s: disassemble status [%s]", mpp->alias, params);

	/*
	 * features
	 */
	p += get_word(p, &word);

	if (!word)
		return 1;

	num_feature_args = atoi(word);
	FREE(word);

	for (i = 0; i < num_feature_args; i++) {
		if (i == 1) {
			p += get_word(p, &word);

			if (!word)
				return 1;

			mpp->queuedio = atoi(word);
			FREE(word);
			continue;
		}
		/* unknown */
		p += get_word(p, NULL);
	}
	/*
	 * hwhandler
	 */
	p += get_word(p, &word);

	if (!word)
		return 1;

	num_hwhandler_args = atoi(word);
	FREE(word);

	for (i = 0; i < num_hwhandler_args; i++)
		p += get_word(p, NULL);

	/*
	 * nb of path groups
	 */
	p += get_word(p, &word);

	if (!word)
		return 1;

	num_pg = atoi(word);
	FREE(word);

	if (num_pg == 0)
		return 0;

	/*
	 * next pg to try
	 */
	p += get_word(p, NULL);

	if (VECTOR_SIZE(mpp->pg) < num_pg)
		return 1;

	for (i = 0; i < num_pg; i++) {
		pgp = VECTOR_SLOT(mpp->pg, i);
		/*
		 * PG status
		 */
		p += get_word(p, &word);

		if (!word)
			return 1;

		switch (*word) {
		case 'D':
			pgp->status = PGSTATE_DISABLED;
			break;
		case 'A':
			pgp->status = PGSTATE_ACTIVE;
			break;
		case 'E':
			pgp->status = PGSTATE_ENABLED;
			break;
		default:
			pgp->status = PGSTATE_UNDEF;
			break;
		}
		FREE(word);

		/*
		 * PG Status (discarded, would be '0' anyway)
		 */
		p += get_word(p, NULL);

		p += get_word(p, &word);

		if (!word)
			return 1;

		num_paths = atoi(word);
		FREE(word);

		p += get_word(p, &word);

		if (!word)
			return 1;

		num_pg_args = atoi(word);
		FREE(word);

		if (VECTOR_SIZE(pgp->paths) < num_paths)
			return 1;

		for (j = 0; j < num_paths; j++) {
			pp = VECTOR_SLOT(pgp->paths, j);
			/*
			 * path
			 */
			p += get_word(p, NULL);

			/*
			 * path status
			 */
			p += get_word(p, &word);

			if (!word)
				return 1;

			switch (*word) {
			case 'F':
				pp->dmstate = PSTATE_FAILED;
				break;
			case 'A':
				pp->dmstate = PSTATE_ACTIVE;
				break;
			default:
				break;
			}
			FREE(word);
			/*
			 * fail count
			 */
			p += get_word(p, &word);

			if (!word)
				return 1;

			pp->failcount = atoi(word);
			FREE(word);

			/*
			 * selector args
			 */
			for (k = 0; k < num_pg_args; k++) {
				if (!strncmp(mpp->selector,
					     "least-pending", 13)) {
					p += get_word(p, &word);
					if (sscanf(word,"%d:*d",
						   &def_minio) == 1 &&
					    def_minio != mpp->minio)
							mpp->minio = def_minio;
				} else
					p += get_word(p, NULL);
			}
		}
	}
	return 0;
}
