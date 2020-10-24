ev_add_map (char * dev, char * alias, struct vectors * vecs)
{
	char * refwwid;
	struct multipath * mpp;
	int map_present;
	int r = 1;

	map_present = dm_map_present(alias);

	if (map_present && dm_type(alias, TGT_MPATH) <= 0) {
		condlog(4, "%s: not a multipath map", alias);
		return 0;
	}

	mpp = find_mp_by_alias(vecs->mpvec, alias);

	if (mpp) {
		/*
		 * Not really an error -- we generate our own uevent
		 * if we create a multipath mapped device as a result
		 * of uev_add_path
		 */
		if (conf->reassign_maps) {
			condlog(3, "%s: Reassign existing device-mapper devices",
				alias);
			dm_reassign(alias);
		}
		return 0;
	}
	condlog(2, "%s: adding map", alias);

	/*
	 * now we can register the map
	 */
	if (map_present && (mpp = add_map_without_path(vecs, alias))) {
		sync_map_state(mpp);
		condlog(2, "%s: devmap %s registered", alias, dev);
		return 0;
	}
	r = get_refwwid(dev, DEV_DEVMAP, vecs->pathvec, &refwwid);

	if (refwwid) {
		r = coalesce_paths(vecs, NULL, refwwid, 0);
		dm_lib_release();
	}

	if (!r)
		condlog(2, "%s: devmap %s added", alias, dev);
	else if (r == 2)
		condlog(2, "%s: uev_add_map %s blacklisted", alias, dev);
	else
		condlog(0, "%s: uev_add_map %s failed", alias, dev);

	FREE(refwwid);
	return r;
}
