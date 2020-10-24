extern struct multipath *
add_map_with_path (struct vectors * vecs,
		   struct path * pp, int add_vec)
{
	struct multipath * mpp;

	if (!(mpp = alloc_multipath()))
		return NULL;

	mpp->mpe = find_mpe(pp->wwid);
	mpp->hwe = pp->hwe;

	strcpy(mpp->wwid, pp->wwid);
	if (select_alias(mpp))
		goto out;
	mpp->size = pp->size;

	if (adopt_paths(vecs->pathvec, mpp, 1))
		goto out;

	if (add_vec) {
		if (!vector_alloc_slot(vecs->mpvec))
			goto out;

		vector_set_slot(vecs->mpvec, mpp);
	}

	return mpp;

out:
	remove_map(mpp, vecs, PURGE_VEC);
	return NULL;
}
