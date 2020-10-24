extern int
one_path_per_group (struct multipath * mp)
{
	int i;
	struct path * pp;
	struct pathgroup * pgp;

	if (!mp->pg)
		mp->pg = vector_alloc();

	if (!mp->pg)
		return 1;

	for (i = 0; i < VECTOR_SIZE(mp->paths); i++) {
		pp = VECTOR_SLOT(mp->paths, i);
		pgp = alloc_pathgroup();

		if (!pgp)
			goto out;

		if (store_pathgroup(mp->pg, pgp))
			goto out;

		if (store_path(pgp->paths, pp))
			goto out;
	}
	sort_pathgroups(mp);
	free_pathvec(mp->paths, KEEP_PATHS);
	mp->paths = NULL;
	return 0;
out:
	free_pgvec(mp->pg, KEEP_PATHS);
	mp->pg = NULL;
	return 1;
}
