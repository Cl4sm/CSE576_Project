extern int
_dm_flush_map (const char * mapname, int need_sync)
{
	int r;

	if (!dm_map_present(mapname))
		return 0;

	if (dm_type(mapname, TGT_MPATH) <= 0)
		return 0; /* nothing to do */

	if (dm_remove_partmaps(mapname, need_sync))
		return 1;

	if (dm_get_opencount(mapname)) {
		condlog(2, "%s: map in use", mapname);
		return 1;
	}

	r = dm_simplecmd_flush(DM_DEVICE_REMOVE, mapname, need_sync);

	if (r) {
		condlog(4, "multipath map %s removed", mapname);
		return 0;
	}
	return 1;
}
