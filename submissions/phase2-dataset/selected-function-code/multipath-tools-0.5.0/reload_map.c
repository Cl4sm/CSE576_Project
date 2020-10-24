{
	char params[PARAMS_SIZE] = {0};
	struct path *pp;
	int i, r;

	update_mpp_paths(mpp, vecs->pathvec);
	if (refresh) {
		vector_foreach_slot (mpp->paths, pp, i) {
			r = pathinfo(pp, conf->hwtable, DI_PRIO);
			if (r) {
				condlog(2, "%s: failed to refresh pathinfo",
					mpp->alias);
				return 1;
			}
		}
	}
	if (setup_map(mpp, params, PARAMS_SIZE)) {
		condlog(0, "%s: failed to setup map", mpp->alias);
		return 1;
	}
	select_action(mpp, vecs->mpvec, 1);

	r = domap(mpp, params);
	if (r == DOMAP_FAIL || r == DOMAP_RETRY) {
		condlog(3, "%s: domap (%u) failure "
			"for reload map", mpp->alias, r);
		return 1;
	}
	if (mpp->no_path_retry != NO_PATH_RETRY_UNDEF) {
		if (mpp->no_path_retry == NO_PATH_RETRY_FAIL)
			dm_queue_if_no_path(mpp->alias, 0);
		else
			dm_queue_if_no_path(mpp->alias, 1);
	}

	return 0;
}
