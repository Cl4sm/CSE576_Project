{
	struct dm_task *dmt;
	struct dm_deps *dm_deps;
	dev_t ret = 0;

	if ((dmt = dm_task_create(DM_DEVICE_DEPS)) == NULL) {
		return ret;
	}
	if (!dm_task_set_name(dmt, devname)) {
		goto out;
	}
	if (!dm_task_run(dmt)) {
		goto out;
	}
	if ((dm_deps = dm_task_get_deps(dmt)) == NULL) {
		goto out;
	}
	if (dm_deps->count > 0) {
		ret = dm_deps->device[0];
	}
out:
	dm_task_destroy(dmt);

	return ret;
}
