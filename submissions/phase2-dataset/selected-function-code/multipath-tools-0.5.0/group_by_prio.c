extern int
group_by_prio (struct multipath * mp)
{
	int i;
	unsigned int prio;
	struct path * pp;
	struct pathgroup * pgp;

	if (!mp->pg)
		mp->pg = vector_alloc();

	if (!mp->pg)
		return 1;

	while (VECTOR_SIZE(mp->paths) > 0) {
		pp = VECTOR_SLOT(mp->paths, 0);
		prio = pp->priority;

		/*
		 * Find the position to insert the new path group. All groups
		 * are ordered by the priority value (higher value first).
		 */
		vector_foreach_slot(mp->pg, pgp, i) {
			pp  = VECTOR_SLOT(pgp->paths, 0);

			if (prio > pp->priority)
				break;
		}

		/*
		 * Initialize the new path group.
		 */
		pgp = alloc_pathgroup();

		if (!pgp)
			goto out;

		if (store_path(pgp->paths, VECTOR_SLOT(mp->paths, 0)))
				goto out;

		vector_del_slot(mp->paths, 0);

		/*
		 * Store the new path group into the vector.
		 */
		if (i < VECTOR_SIZE(mp->pg)) {
			if (!vector_insert_slot(mp->pg, i, pgp))
				goto out;
		} else {
			if (store_pathgroup(mp->pg, pgp))
				goto out;
		}

		/*
		 * add the other paths with the same prio
		 */
		vector_foreach_slot(mp->paths, pp, i) {
			if (pp->priority == prio) {
				if (store_path(pgp->paths, pp))
					goto out;

				vector_del_slot(mp->paths, i);
				i--;
			}
		}
	}
	free_pathvec(mp->paths, KEEP_PATHS);
	mp->paths = NULL;
	return 0;
out:
	free_pgvec(mp->pg, KEEP_PATHS);
	mp->pg = NULL;
	return 1;

}
