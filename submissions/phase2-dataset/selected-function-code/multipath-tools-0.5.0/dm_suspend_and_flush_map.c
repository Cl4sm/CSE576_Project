extern int
dm_suspend_and_flush_map (const char * mapname)
{
	int s = 0, queue_if_no_path = 0;
	unsigned long long mapsize;
	char params[PARAMS_SIZE] = {0};

	if (!dm_map_present(mapname))
		return 0;

	if (dm_type(mapname, TGT_MPATH) <= 0)
		return 0; /* nothing to do */

	if (!dm_get_map(mapname, &mapsize, params)) {
		if (strstr(params, "queue_if_no_path"))
			queue_if_no_path = 1;
	}

	if (queue_if_no_path)
		s = dm_queue_if_no_path((char *)mapname, 0);
	/* Leave queue_if_no_path alone if unset failed */
	if (s)
		queue_if_no_path = 0;
	else
		s = dm_simplecmd_flush(DM_DEVICE_SUSPEND, mapname, 0);

	if (!dm_flush_map(mapname)) {
		condlog(4, "multipath map %s removed", mapname);
		return 0;
	}
	condlog(2, "failed to remove multipath map %s", mapname);
	dm_simplecmd_noflush(DM_DEVICE_RESUME, mapname);
	if (queue_if_no_path)
		s = dm_queue_if_no_path((char *)mapname, 1);
	return 1;
}
