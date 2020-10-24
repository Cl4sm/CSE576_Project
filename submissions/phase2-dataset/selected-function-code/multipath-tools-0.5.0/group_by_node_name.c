group_by_node_name (struct multipath * mp) {
	int i, j;
	int * bitmap;
	struct path * pp;
	struct pathgroup * pgp;
	struct path * pp2;

	if (!mp->pg)
		mp->pg = vector_alloc();

	if (!mp->pg)
		return 1;

	/* init the bitmap */
	bitmap = (int *)MALLOC(VECTOR_SIZE(mp->paths) * sizeof (int));

	if (!bitmap)
		goto out;

	for (i = 0; i < VECTOR_SIZE(mp->paths); i++) {

		if (bitmap[i])
			continue;

		pp = VECTOR_SLOT(mp->paths, i);

		/* here, we really got a new pg */
		pgp = alloc_pathgroup();

		if (!pgp)
			goto out1;

		if (store_pathgroup(mp->pg, pgp))
			goto out1;

		/* feed the first path */
		if (store_path(pgp->paths, pp))
			goto out1;

		bitmap[i] = 1;

		for (j = i + 1; j < VECTOR_SIZE(mp->paths); j++) {

			if (bitmap[j])
				continue;

			pp2 = VECTOR_SLOT(mp->paths, j);

			if (!strncmp(pp->tgt_node_name, pp2->tgt_node_name,
					NODE_NAME_SIZE)) {
				if (store_path(pgp->paths, pp2))
					goto out1;

				bitmap[j] = 1;
			}
		}
	}
	FREE(bitmap);
	sort_pathgroups(mp);
	free_pathvec(mp->paths, KEEP_PATHS);
	mp->paths = NULL;
	return 0;
out1:
	FREE(bitmap);
out:
	free_pgvec(mp->pg, KEEP_PATHS);
	mp->pg = NULL;
	return 1;
}
