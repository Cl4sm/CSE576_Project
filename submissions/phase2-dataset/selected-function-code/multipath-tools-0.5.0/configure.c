configure (struct vectors * vecs, int start_waiters)
{
	struct multipath * mpp;
	struct path * pp;
	vector mpvec;
	int i;

	if (!vecs->pathvec && !(vecs->pathvec = vector_alloc()))
		return 1;

	if (!vecs->mpvec && !(vecs->mpvec = vector_alloc()))
		return 1;

	if (!(mpvec = vector_alloc()))
		return 1;

	/*
	 * probe for current path (from sysfs) and map (from dm) sets
	 */
	path_discovery(vecs->pathvec, conf, DI_ALL);

	vector_foreach_slot (vecs->pathvec, pp, i){
		if (filter_path(conf, pp) > 0){
			vector_del_slot(vecs->pathvec, i);
			free_path(pp);
			i--;
		}
		else
			pp->checkint = conf->checkint;
	}
	if (map_discovery(vecs))
		return 1;

	/*
	 * create new set of maps & push changed ones into dm
	 */
	if (coalesce_paths(vecs, mpvec, NULL, 1))
		return 1;

	/*
	 * may need to remove some maps which are no longer relevant
	 * e.g., due to blacklist changes in conf file
	 */
	if (coalesce_maps(vecs, mpvec))
		return 1;

	dm_lib_release();

	sync_maps_state(mpvec);
	vector_foreach_slot(mpvec, mpp, i){
		remember_wwid(mpp->wwid);
		update_map_pr(mpp);
	}

	/*
	 * purge dm of old maps
	 */
	remove_maps(vecs);

	/*
	 * save new set of maps formed by considering current path state
	 */
	vector_free(vecs->mpvec);
	vecs->mpvec = mpvec;

	/*
	 * start dm event waiter threads for these new maps
	 */
	vector_foreach_slot(vecs->mpvec, mpp, i) {
		if (setup_multipath(vecs, mpp))
			return 1;
		if (start_waiters)
			if (start_waiter_thread(mpp, vecs))
				return 1;
	}
	return 0;
}
