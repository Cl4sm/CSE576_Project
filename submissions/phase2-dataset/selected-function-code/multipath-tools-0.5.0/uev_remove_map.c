uev_remove_map (struct uevent * uev, struct vectors * vecs)
{
	char *alias;
	int minor;
	struct multipath *mpp;

	condlog(2, "%s: remove map (uevent)", uev->kernel);
	alias = uevent_get_dm_name(uev);
	if (!alias) {
		condlog(3, "%s: No DM_NAME in uevent, ignoring", uev->kernel);
		return 0;
	}
	minor = uevent_get_minor(uev);
	mpp = find_mp_by_minor(vecs->mpvec, minor);

	if (!mpp) {
		condlog(2, "%s: devmap not registered, can't remove",
			uev->kernel);
		goto out;
	}
	if (strcmp(mpp->alias, alias)) {
		condlog(2, "%s: minor number mismatch (map %d, event %d)",
			mpp->alias, mpp->dmi->minor, minor);
		goto out;
	}

	orphan_paths(vecs->pathvec, mpp);
	remove_map_and_stop_waiter(mpp, vecs, 1);
out:
	FREE(alias);
	return 0;
}
