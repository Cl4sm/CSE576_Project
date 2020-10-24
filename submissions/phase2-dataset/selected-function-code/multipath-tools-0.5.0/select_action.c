static void
select_action (struct multipath * mpp, vector curmp, int force_reload)
{
	struct multipath * cmpp;
	struct multipath * cmpp_by_name;

	cmpp = find_mp_by_wwid(curmp, mpp->wwid);
	cmpp_by_name = find_mp_by_alias(curmp, mpp->alias);

	if (!cmpp_by_name) {
		if (cmpp) {
			condlog(2, "%s: rename %s to %s", mpp->wwid,
				cmpp->alias, mpp->alias);
			strncpy(mpp->alias_old, cmpp->alias, WWID_SIZE);
			mpp->action = ACT_RENAME;
			return;
		}
		mpp->action = ACT_CREATE;
		condlog(3, "%s: set ACT_CREATE (map does not exist)",
			mpp->alias);
		return;
	}

	if (!cmpp) {
		condlog(2, "%s: remove (wwid changed)", mpp->alias);
		dm_flush_map(mpp->alias);
		strncpy(cmpp_by_name->wwid, mpp->wwid, WWID_SIZE);
		drop_multipath(curmp, cmpp_by_name->wwid, KEEP_PATHS);
		mpp->action = ACT_CREATE;
		condlog(3, "%s: set ACT_CREATE (map wwid change)",
			mpp->alias);
		return;
	}

	if (cmpp != cmpp_by_name) {
		condlog(2, "%s: unable to rename %s to %s (%s is used by %s)",
			mpp->wwid, cmpp->alias, mpp->alias,
			mpp->alias, cmpp_by_name->wwid);
		mpp->action = ACT_NOTHING;
		return;
	}

	if (pathcount(mpp, PATH_UP) == 0) {
		mpp->action = ACT_NOTHING;
		condlog(3, "%s: set ACT_NOTHING (no usable path)",
			mpp->alias);
		return;
	}
	if (force_reload) {
		mpp->action = ACT_RELOAD;
		condlog(3, "%s: set ACT_RELOAD (forced by user)",
			mpp->alias);
		return;
	}
	if (cmpp->size != mpp->size) {
		mpp->action = ACT_RESIZE;
		condlog(3, "%s: set ACT_RESIZE (size change)",
			mpp->alias);
		return;
	}
	if (!mpp->no_path_retry &&
	    (strlen(cmpp->features) != strlen(mpp->features) ||
	     strcmp(cmpp->features, mpp->features))) {
		mpp->action =  ACT_RELOAD;
		condlog(3, "%s: set ACT_RELOAD (features change)",
			mpp->alias);
		return;
	}
	if (mpp->retain_hwhandler != RETAIN_HWHANDLER_ON &&
            (strlen(cmpp->hwhandler) != strlen(mpp->hwhandler) ||
	     strncmp(cmpp->hwhandler, mpp->hwhandler,
		    strlen(mpp->hwhandler)))) {
		mpp->action = ACT_RELOAD;
		condlog(3, "%s: set ACT_RELOAD (hwhandler change)",
			mpp->alias);
		return;
	}
	if (!cmpp->selector || strncmp(cmpp->selector, mpp->selector,
		    strlen(mpp->selector))) {
		mpp->action = ACT_RELOAD;
		condlog(3, "%s: set ACT_RELOAD (selector change)",
			mpp->alias);
		return;
	}
	if (cmpp->minio != mpp->minio) {
		mpp->action = ACT_RELOAD;
		condlog(3, "%s: set ACT_RELOAD (minio change, %u->%u)",
			mpp->alias, cmpp->minio, mpp->minio);
		return;
	}
	if (!cmpp->pg || VECTOR_SIZE(cmpp->pg) != VECTOR_SIZE(mpp->pg)) {
		mpp->action = ACT_RELOAD;
		condlog(3, "%s: set ACT_RELOAD (path group number change)",
			mpp->alias);
		return;
	}
	if (pgcmp(mpp, cmpp)) {
		mpp->action = ACT_RELOAD;
		condlog(3, "%s: set ACT_RELOAD (path group topology change)",
			mpp->alias);
		return;
	}
	if (cmpp->nextpg != mpp->bestpg) {
		mpp->action = ACT_SWITCHPG;
		condlog(3, "%s: set ACT_SWITCHPG (next path group change)",
			mpp->alias);
		return;
	}
	mpp->action = ACT_NOTHING;
	condlog(3, "%s: set ACT_NOTHING (map unchanged)",
		mpp->alias);
	return;
}
