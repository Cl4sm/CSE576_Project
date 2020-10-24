coalesce_paths (struct vectors * vecs, vector newmp, char * refwwid, int force_reload)
{
	int r = 1;
	int k, i;
	char empty_buff[WWID_SIZE];
	char params[PARAMS_SIZE];
	struct multipath * mpp;
	struct path * pp1;
	struct path * pp2;
	vector curmp = vecs->mpvec;
	vector pathvec = vecs->pathvec;

	memset(empty_buff, 0, WWID_SIZE);

	if (force_reload) {
		vector_foreach_slot (pathvec, pp1, k) {
			pp1->mpp = NULL;
		}
	}
	vector_foreach_slot (pathvec, pp1, k) {
		/* skip this path for some reason */

		/* 1. if path has no unique id or wwid blacklisted */
		if (memcmp(empty_buff, pp1->wwid, WWID_SIZE) == 0 ||
		    filter_path(conf, pp1) > 0) {
			orphan_path(pp1, "wwid blacklisted");
			continue;
		}

		/* 2. if path already coalesced */
		if (pp1->mpp)
			continue;

		/* 3. if path has disappeared */
		if (!pp1->size) {
			orphan_path(pp1, "invalid size");
			continue;
		}

		/* 4. path is out of scope */
		if (refwwid && strncmp(pp1->wwid, refwwid, WWID_SIZE))
			continue;

		/*
		 * at this point, we know we really got a new mp
		 */
		mpp = add_map_with_path(vecs, pp1, 0);
		if (!mpp)
			return 1;

		if (pp1->priority == PRIO_UNDEF)
			mpp->action = ACT_REJECT;

		if (!mpp->paths) {
			condlog(0, "%s: skip coalesce (no paths)", mpp->alias);
			remove_map(mpp, vecs, 0);
			continue;
		}

		for (i = k + 1; i < VECTOR_SIZE(pathvec); i++) {
			pp2 = VECTOR_SLOT(pathvec, i);

			if (strcmp(pp1->wwid, pp2->wwid))
				continue;

			if (!pp2->size)
				continue;

			if (pp2->size != mpp->size) {
				/*
				 * ouch, avoid feeding that to the DM
				 */
				condlog(0, "%s: size %llu, expected %llu. "
					"Discard", pp2->dev_t, pp2->size,
					mpp->size);
				mpp->action = ACT_REJECT;
			}
			if (pp2->priority == PRIO_UNDEF)
				mpp->action = ACT_REJECT;
		}
		verify_paths(mpp, vecs, NULL);

		params[0] = '\0';
		if (setup_map(mpp, params, PARAMS_SIZE)) {
			remove_map(mpp, vecs, 0);
			continue;
		}

		if (mpp->action == ACT_UNDEF)
			select_action(mpp, curmp, force_reload);

		r = domap(mpp, params);

		if (r == DOMAP_FAIL || r == DOMAP_RETRY) {
			condlog(3, "%s: domap (%u) failure "
				   "for create/reload map",
				mpp->alias, r);
			if (r == DOMAP_FAIL) {
				condlog(2, "%s: %s map",
					mpp->alias, (mpp->action == ACT_CREATE)?
					"ignoring" : "removing");
				remove_map(mpp, vecs, 0);
				continue;
			} else /* if (r == DOMAP_RETRY) */
				return r;
		}
		if (r == DOMAP_DRY)
			continue;

		if (!conf->daemon && !conf->allow_queueing && !check_daemon()) {
			if (mpp->no_path_retry != NO_PATH_RETRY_UNDEF &&
			    mpp->no_path_retry != NO_PATH_RETRY_FAIL)
				condlog(3, "%s: multipathd not running, unset "
					"queue_if_no_path feature", mpp->alias);
			if (!dm_queue_if_no_path(mpp->alias, 0))
				remove_feature(&mpp->features,
					       "queue_if_no_path");
		}
		else if (mpp->no_path_retry != NO_PATH_RETRY_UNDEF) {
			if (mpp->no_path_retry == NO_PATH_RETRY_FAIL) {
				condlog(3, "%s: unset queue_if_no_path feature",
					mpp->alias);
				if (!dm_queue_if_no_path(mpp->alias, 0))
					remove_feature(&mpp->features,
						       "queue_if_no_path");
			} else {
				condlog(3, "%s: set queue_if_no_path feature",
					mpp->alias);
				if (!dm_queue_if_no_path(mpp->alias, 1))
					add_feature(&mpp->features,
						    "queue_if_no_path");
			}
		}

		if (!conf->daemon && mpp->action != ACT_NOTHING)
			print_multipath_topology(mpp, conf->verbosity);

		if (newmp) {
			if (mpp->action != ACT_REJECT) {
				if (!vector_alloc_slot(newmp))
					return 1;
				vector_set_slot(newmp, mpp);
			}
			else
				remove_map(mpp, vecs, 0);
		}
	}
	/*
	 * Flush maps with only dead paths (ie not in sysfs)
	 * Keep maps with only failed paths
	 */
	if (newmp) {
		vector_foreach_slot (newmp, mpp, i) {
			char alias[WWID_SIZE];
			int j;

			if (!deadmap(mpp))
				continue;

			strncpy(alias, mpp->alias, WWID_SIZE);

			if ((j = find_slot(newmp, (void *)mpp)) != -1)
				vector_del_slot(newmp, j);

			remove_map(mpp, vecs, 0);

			if (dm_flush_map(alias))
				condlog(2, "%s: remove failed (dead)",
					alias);
			else
				condlog(2, "%s: remove (dead)", alias);
		}
	}
	return 0;
}
