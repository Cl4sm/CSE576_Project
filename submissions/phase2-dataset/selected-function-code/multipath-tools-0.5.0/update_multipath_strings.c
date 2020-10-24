extern int
update_multipath_strings (struct multipath *mpp, vector pathvec)
{
	if (!mpp)
		return 1;

	condlog(4, "%s: %s", mpp->alias, __FUNCTION__);

	free_multipath_attributes(mpp);
	free_pgvec(mpp->pg, KEEP_PATHS);
	mpp->pg = NULL;

	if (update_multipath_table(mpp, pathvec))
		return 1;

	if (update_multipath_status(mpp))
		return 1;

	return 0;
}
