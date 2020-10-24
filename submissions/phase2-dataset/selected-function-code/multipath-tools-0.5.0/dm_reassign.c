{
	struct dm_deps *deps;
	struct dm_task *dmt;
	struct dm_info info;
	char dev_t[32], dm_dep[32];
	int r = 0, i;

	if (dm_dev_t(mapname, &dev_t[0], 32)) {
		condlog(3, "%s: failed to get device number", mapname);
		return 1;
	}

	if (!(dmt = dm_task_create(DM_DEVICE_DEPS))) {
		condlog(3, "%s: couldn't make dm task", mapname);
		return 0;
	}

	if (!dm_task_set_name(dmt, mapname))
		goto out;

	dm_task_no_open_count(dmt);

	if (!dm_task_run(dmt))
		goto out;

	if (!dm_task_get_info(dmt, &info))
		goto out;

	if (!(deps = dm_task_get_deps(dmt)))
		goto out;

	if (!info.exists)
		goto out;

	for (i = 0; i < deps->count; i++) {
		sprintf(dm_dep, "%d:%d",
			major(deps->device[i]),
			minor(deps->device[i]));
		sysfs_check_holders(dm_dep, dev_t);
	}

	dm_task_destroy (dmt);

	r = 1;
out:
	return r;
}
