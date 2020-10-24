extern int
domap (struct multipath * mpp, char * params)
{
	int r = 0;

	/*
	 * last chance to quit before touching the devmaps
	 */
	if (conf->dry_run && mpp->action != ACT_NOTHING) {
		print_multipath_topology(mpp, conf->verbosity);
		return DOMAP_DRY;
	}

	switch (mpp->action) {
	case ACT_REJECT:
	case ACT_NOTHING:
		return DOMAP_EXIST;

	case ACT_SWITCHPG:
		dm_switchgroup(mpp->alias, mpp->bestpg);
		/*
		 * we may have avoided reinstating paths because there where in
		 * active or disabled PG. Now that the topology has changed,
		 * retry.
		 */
		reinstate_paths(mpp);
		return DOMAP_EXIST;

	case ACT_CREATE:
		if (lock_multipath(mpp, 1)) {
			condlog(3, "%s: failed to create map (in use)",
				mpp->alias);
			return DOMAP_RETRY;
		}

		if (dm_map_present(mpp->alias)) {
			condlog(3, "%s: map already present", mpp->alias);
			lock_multipath(mpp, 0);
			break;
		}

		r = dm_addmap_create(mpp, params);

		lock_multipath(mpp, 0);
		break;

	case ACT_RELOAD:
		r = dm_addmap_reload(mpp, params);
		if (r)
			r = dm_simplecmd_noflush(DM_DEVICE_RESUME, mpp->alias);
		break;

	case ACT_RESIZE:
		r = dm_addmap_reload(mpp, params);
		if (r)
			r = dm_simplecmd_flush(DM_DEVICE_RESUME, mpp->alias, 1);
		break;

	case ACT_RENAME:
		r = dm_rename(mpp->alias_old, mpp->alias);
		break;

	default:
		break;
	}

	if (r) {
		/*
		 * DM_DEVICE_CREATE, DM_DEVICE_RENAME, or DM_DEVICE_RELOAD
		 * succeeded
		 */
		if (mpp->action == ACT_CREATE)
			remember_wwid(mpp->wwid);
		if (!conf->daemon) {
			/* multipath client mode */
			dm_switchgroup(mpp->alias, mpp->bestpg);
		} else  {
			/* multipath daemon mode */
			mpp->stat_map_loads++;
			condlog(2, "%s: load table [0 %llu %s %s]", mpp->alias,
				mpp->size, TGT_MPATH, params);
			/*
			 * Required action is over, reset for the stateful daemon.
			 * But don't do it for creation as we use in the caller the
			 * mpp->action to figure out whether to start the watievent checker.
			 */
			if (mpp->action != ACT_CREATE)
				mpp->action = ACT_NOTHING;
		}
		dm_setgeometry(mpp);
		return DOMAP_OK;
	}
	return DOMAP_FAIL;
}
