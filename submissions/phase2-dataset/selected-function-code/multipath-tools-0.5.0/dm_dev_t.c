static int
dm_dev_t (const char * mapname, char * dev_t, int len)
{
	int r = 1;
	struct dm_task *dmt;
	struct dm_info info;

	if (!(dmt = dm_task_create(DM_DEVICE_INFO)))
		return 0;

	if (!dm_task_set_name(dmt, mapname))
		goto out;

	if (!dm_task_run(dmt))
		goto out;

	if (!dm_task_get_info(dmt, &info))
		goto out;

	r = info.open_count;
	if (snprintf(dev_t, len, "%i:%i", info.major, info.minor) > len)
		    goto out;

	r = 0;
out:
	dm_task_destroy(dmt);
	return r;
}
