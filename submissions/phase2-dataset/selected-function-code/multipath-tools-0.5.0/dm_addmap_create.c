dm_addmap_create (struct multipath *mpp, char * params) {
	int ro;

	for (ro = 0; ro <= 1; ro++) {
		int err;

		if (dm_addmap(DM_DEVICE_CREATE, TGT_MPATH, mpp, params, 1, ro))
			return 1;
		/*
		 * DM_DEVICE_CREATE is actually DM_DEV_CREATE + DM_TABLE_LOAD.
		 * Failing the second part leaves an empty map. Clean it up.
		 */
		err = errno;
		if (dm_map_present(mpp->alias)) {
			condlog(3, "%s: failed to load map (a path might be in use)", mpp->alias);
			dm_flush_map_nosync(mpp->alias);
		}
		if (err != EROFS)
			break;
	}
	return 0;
}
