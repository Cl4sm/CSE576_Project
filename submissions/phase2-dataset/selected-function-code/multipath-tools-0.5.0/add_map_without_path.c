add_map_without_path (struct vectors * vecs, char * alias)
{
	struct multipath * mpp = alloc_multipath();

	if (!mpp || !alias)
		return NULL;

	mpp->alias = STRDUP(alias);

	if (setup_multipath(vecs, mpp))
		return NULL; /* mpp freed in setup_multipath */

	if (adopt_paths(vecs->pathvec, mpp, 1))
		goto out;

	if (!vector_alloc_slot(vecs->mpvec))
		goto out;

	vector_set_slot(vecs->mpvec, mpp);

	if (start_waiter_thread(mpp, vecs))
		goto out;

	return mpp;
out:
	remove_map(mpp, vecs, PURGE_VEC);
	return NULL;
}
