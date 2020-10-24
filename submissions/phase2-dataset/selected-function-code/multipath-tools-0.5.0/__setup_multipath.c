__setup_multipath (struct vectors * vecs, struct multipath * mpp, int reset)
{
	if (dm_get_info(mpp->alias, &mpp->dmi)) {
		/* Error accessing table */
		condlog(3, "%s: cannot access table", mpp->alias);
		goto out;
	}

	if (!dm_map_present(mpp->alias)) {
		/* Table has been removed */
		condlog(3, "%s: table does not exist", mpp->alias);
		goto out;
	}

	if (update_multipath_strings(mpp, vecs->pathvec)) {
		condlog(0, "%s: failed to setup multipath", mpp->alias);
		goto out;
	}

	set_multipath_wwid(mpp);
	mpp->mpe = find_mpe(mpp->wwid);
	condlog(3, "%s: discover", mpp->alias);

	if (!mpp->hwe)
		mpp->hwe = extract_hwe_from_path(mpp);
	if (!mpp->hwe) {
		condlog(3, "%s: no hardware entry found, using defaults",
			mpp->alias);
	}
	if (reset) {
		select_rr_weight(mpp);
		select_pgfailback(mpp);
		set_no_path_retry(mpp);
		select_flush_on_last_del(mpp);
	}

	return 0;
out:
	remove_map(mpp, vecs, PURGE_VEC);
	return 1;
}
