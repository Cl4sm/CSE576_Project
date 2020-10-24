one_group (struct multipath * mp)	/* aka multibus */
{
	struct pathgroup * pgp;

	if (VECTOR_SIZE(mp->paths) < 0)
		return 0;

	if (!mp->pg)
		mp->pg = vector_alloc();

	if (!mp->pg)
		return 1;

	if (VECTOR_SIZE(mp->paths) > 0) {
		pgp = alloc_pathgroup();

		if (!pgp)
			goto out;

		vector_free(pgp->paths);
		pgp->paths = mp->paths;
		mp->paths = NULL;

		if (store_pathgroup(mp->pg, pgp))
			goto out;
	}

	return 0;
out:
	free_pgvec(mp->pg, KEEP_PATHS);
	mp->pg = NULL;
	return 1;
}
