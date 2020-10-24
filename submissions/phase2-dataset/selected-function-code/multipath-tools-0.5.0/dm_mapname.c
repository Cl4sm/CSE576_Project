dm_mapname(int major, int minor)
{
	struct dm_task *dmt;
	char *mapname = NULL;
	const char *map;

	if (!(dmt = dm_task_create(DM_DEVICE_INFO)))
		return NULL;

	dm_task_no_open_count(dmt);
	dm_task_set_major(dmt, major);
	dm_task_set_minor(dmt, minor);

	if (!dm_task_run(dmt))
		goto out;

	map = dm_task_get_name(dmt);
	if (map && strlen(map))
		mapname = strdup(map);

out:
	dm_task_destroy(dmt);
	return mapname;
}
