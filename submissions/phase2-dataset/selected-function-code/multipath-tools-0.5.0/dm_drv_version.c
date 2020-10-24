int
dm_drv_version (unsigned int * version, char * str)
{
	int r = 2;
	struct dm_task *dmt;
	struct dm_versions *target;
	struct dm_versions *last_target;
	unsigned int *v;

	version[0] = 0;
	version[1] = 0;
	version[2] = 0;

	if (!(dmt = dm_task_create(DM_DEVICE_LIST_VERSIONS)))
		return 1;

	dm_task_no_open_count(dmt);

	if (!dm_task_run(dmt)) {
		condlog(0, "Can not communicate with kernel DM");
		goto out;
	}
	target = dm_task_get_versions(dmt);

	do {
		last_target = target;
		if (!strncmp(str, target->name, strlen(str))) {
			r = 1;
			break;
		}
		target = (void *) target + target->next;
	} while (last_target != target);

	if (r == 2) {
		condlog(0, "DM %s kernel driver not loaded", str);
		goto out;
	}
	v = target->version;
        version[0] = v[0];
        version[1] = v[1];
        version[2] = v[2];
	r = 0;
out:
	dm_task_destroy(dmt);
	return r;
}
