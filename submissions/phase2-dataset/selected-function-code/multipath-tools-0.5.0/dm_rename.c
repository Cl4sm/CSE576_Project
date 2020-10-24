dm_rename (char * old, char * new)
{
	int r = 0;
	struct dm_task *dmt;

	if (dm_rename_partmaps(old, new))
		return r;

	if (!(dmt = dm_task_create(DM_DEVICE_RENAME)))
		return r;

	if (!dm_task_set_name(dmt, old))
		goto out;

	if (!dm_task_set_newname(dmt, new))
		goto out;

	dm_task_no_open_count(dmt);

	if (!dm_task_set_cookie(dmt, &conf->cookie, (conf->daemon)? DM_UDEV_DISABLE_LIBRARY_FALLBACK : 0))
		goto out;
	if (!dm_task_run(dmt))
		goto out;

	r = 1;
out:
	dm_task_destroy(dmt);
	return r;
}
