deadmap (struct multipath * mpp)
{
	int i, j;
	struct pathgroup * pgp;
	struct path * pp;

	if (!mpp->pg)
		return 1;

	vector_foreach_slot (mpp->pg, pgp, i) {
		if (!pgp->paths)
			continue;

		vector_foreach_slot (pgp->paths, pp, j)
			if (strlen(pp->dev))
				return 0; /* alive */
	}

	return 1; /* dead */
}
