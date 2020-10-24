extern int
dm_simplecmd (int task, const char *name, int no_flush, uint32_t *cookie) {
	int r = 0;
	int udev_wait_flag = (task == DM_DEVICE_RESUME ||
			      task == DM_DEVICE_REMOVE);
	struct dm_task *dmt;

	if (!(dmt = dm_task_create(task)))
		return 0;

	if (!dm_task_set_name(dmt, name))
		goto out;

	dm_task_no_open_count(dmt);
	dm_task_skip_lockfs(dmt);

	if (no_flush)
		dm_task_no_flush(dmt);

	if (udev_wait_flag && !dm_task_set_cookie(dmt, cookie, (udev_sync)? 0 : DM_UDEV_DISABLE_LIBRARY_FALLBACK))
		goto out;
	r = dm_task_run(dmt);

	out:
	dm_task_destroy(dmt);
	return r;
}
