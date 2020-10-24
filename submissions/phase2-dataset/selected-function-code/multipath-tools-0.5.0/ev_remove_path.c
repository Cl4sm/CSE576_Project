ev_remove_path (struct path *pp, struct vectors * vecs)
{
	struct multipath * mpp;
	int i, retval = 0;
	char params[PARAMS_SIZE] = {0};

	/*
	 * avoid referring to the map of an orphaned path
	 */
	if ((mpp = pp->mpp)) {
		/*
		 * transform the mp->pg vector of vectors of paths
		 * into a mp->params string to feed the device-mapper
		 */
		if (update_mpp_paths(mpp, vecs->pathvec)) {
			condlog(0, "%s: failed to update paths",
				mpp->alias);
			goto fail;
		}
		if ((i = find_slot(mpp->paths, (void *)pp)) != -1)
			vector_del_slot(mpp->paths, i);

		/*
		 * remove the map IFF removing the last path
		 */
		if (VECTOR_SIZE(mpp->paths) == 0) {
			char alias[WWID_SIZE];

			/*
			 * flush_map will fail if the device is open
			 */
			strncpy(alias, mpp->alias, WWID_SIZE);
			if (mpp->flush_on_last_del == FLUSH_ENABLED) {
				condlog(2, "%s Last path deleted, disabling queueing", mpp->alias);
				mpp->retry_tick = 0;
				mpp->no_path_retry = NO_PATH_RETRY_FAIL;
				mpp->flush_on_last_del = FLUSH_IN_PROGRESS;
				dm_queue_if_no_path(mpp->alias, 0);
			}
			if (!flush_map(mpp, vecs)) {
				condlog(2, "%s: removed map after"
					" removing all paths",
					alias);
				retval = 0;
				goto out;
			}
			/*
			 * Not an error, continue
			 */
		}

		if (setup_map(mpp, params, PARAMS_SIZE)) {
			condlog(0, "%s: failed to setup map for"
				" removal of path %s", mpp->alias, pp->dev);
			goto fail;
		}
		/*
		 * reload the map
		 */
		mpp->action = ACT_RELOAD;
		if (domap(mpp, params) <= 0) {
			condlog(0, "%s: failed in domap for "
				"removal of path %s",
				mpp->alias, pp->dev);
			retval = 1;
		} else {
			/*
			 * update our state from kernel
			 */
			if (setup_multipath(vecs, mpp)) {
				goto fail;
			}
			sync_map_state(mpp);

			condlog(2, "%s [%s]: path removed from map %s",
				pp->dev, pp->dev_t, mpp->alias);
		}
	}

out:
	if ((i = find_slot(vecs->pathvec, (void *)pp)) != -1)
		vector_del_slot(vecs->pathvec, i);

	free_path(pp);

	return retval;

fail:
	remove_map_and_stop_waiter(mpp, vecs, 1);
	return 1;
}
