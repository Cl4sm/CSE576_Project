int
ev_add_path (struct path * pp, struct vectors * vecs)
{
	struct multipath * mpp;
	char empty_buff[WWID_SIZE] = {0};
	char params[PARAMS_SIZE] = {0};
	int retries = 3;
	int start_waiter = 0;

	/*
	 * need path UID to go any further
	 */
	if (memcmp(empty_buff, pp->wwid, WWID_SIZE) == 0) {
		condlog(0, "%s: failed to get path uid", pp->dev);
		goto fail; /* leave path added to pathvec */
	}
	mpp = pp->mpp = find_mp_by_wwid(vecs->mpvec, pp->wwid);
rescan:
	if (mpp) {
		if ((!pp->size) || (mpp->size != pp->size)) {
			if (!pp->size)
				condlog(0, "%s: failed to add new path %s, "
					"device size is 0",
					mpp->alias, pp->dev);
			else
				condlog(0, "%s: failed to add new path %s, "
					"device size mismatch",
					mpp->alias, pp->dev);
			int i = find_slot(vecs->pathvec, (void *)pp);
			if (i != -1)
				vector_del_slot(vecs->pathvec, i);
			free_path(pp);
			return 1;
		}

		condlog(4,"%s: adopting all paths for path %s",
			mpp->alias, pp->dev);
		if (adopt_paths(vecs->pathvec, mpp, 1))
			goto fail; /* leave path added to pathvec */

		verify_paths(mpp, vecs, NULL);
		mpp->flush_on_last_del = FLUSH_UNDEF;
		mpp->action = ACT_RELOAD;
	}
	else {
		if (!pp->size) {
			condlog(0, "%s: failed to create new map,"
				" device size is 0 ", pp->dev);
			int i = find_slot(vecs->pathvec, (void *)pp);
			if (i != -1)
				vector_del_slot(vecs->pathvec, i);
			free_path(pp);
			return 1;
		}

		condlog(4,"%s: creating new map", pp->dev);
		if ((mpp = add_map_with_path(vecs, pp, 1))) {
			mpp->action = ACT_CREATE;
			/*
			 * We don't depend on ACT_CREATE, as domap will
			 * set it to ACT_NOTHING when complete.
			 */
			start_waiter = 1;
		}
		else
			goto fail; /* leave path added to pathvec */
	}

	/* persistent reseravtion check*/
	mpath_pr_event_handle(pp);	

	/*
	 * push the map to the device-mapper
	 */
	if (setup_map(mpp, params, PARAMS_SIZE)) {
		condlog(0, "%s: failed to setup map for addition of new "
			"path %s", mpp->alias, pp->dev);
		goto fail_map;
	}
	/*
	 * reload the map for the multipath mapped device
	 */
	if (domap(mpp, params) <= 0) {
		condlog(0, "%s: failed in domap for addition of new "
			"path %s", mpp->alias, pp->dev);
		/*
		 * deal with asynchronous uevents :((
		 */
		if (mpp->action == ACT_RELOAD && retries-- > 0) {
			condlog(0, "%s: uev_add_path sleep", mpp->alias);
			sleep(1);
			update_mpp_paths(mpp, vecs->pathvec);
			goto rescan;
		}
		else if (mpp->action == ACT_RELOAD)
			condlog(0, "%s: giving up reload", mpp->alias);
		else
			goto fail_map;
	}
	dm_lib_release();

	/*
	 * update our state from kernel regardless of create or reload
	 */
	if (setup_multipath(vecs, mpp))
		goto fail; /* if setup_multipath fails, it removes the map */

	sync_map_state(mpp);

	if ((mpp->action == ACT_CREATE ||
	     (mpp->action == ACT_NOTHING && start_waiter && !mpp->waiter)) &&
	    start_waiter_thread(mpp, vecs))
			goto fail_map;

	if (retries >= 0) {
		condlog(2, "%s [%s]: path added to devmap %s",
			pp->dev, pp->dev_t, mpp->alias);
		return 0;
	}
	else
		return 1;

fail_map:
	remove_map(mpp, vecs, 1);
fail:
	orphan_path(pp, "failed to add path");
	return 1;
}
