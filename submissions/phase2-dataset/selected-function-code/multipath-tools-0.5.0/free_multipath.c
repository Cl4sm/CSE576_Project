free_multipath (struct multipath * mpp, enum free_path_mode free_paths)
{
	if (!mpp)
		return;

	free_multipath_attributes(mpp);

	if (mpp->alias) {
		FREE(mpp->alias);
		mpp->alias = NULL;
	}

	if (mpp->dmi) {
		FREE(mpp->dmi);
		mpp->dmi = NULL;
	}

	free_pathvec(mpp->paths, free_paths);
	free_pgvec(mpp->pg, free_paths);
	FREE_PTR(mpp->mpcontext);
	FREE(mpp);
}
