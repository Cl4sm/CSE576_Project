dm_prereq (char * str, int x, int y, int z)
{
	int r = 1;
	struct dm_task *dmt;
	struct dm_versions *target;
	struct dm_versions *last_target;

	if (!(dmt = dm_task_create(DM_DEVICE_LIST_VERSIONS)))
		return 1;

	dm_task_no_open_count(dmt);

	if (!dm_task_run(dmt))
		goto out;

	target = dm_task_get_versions(dmt);

	/* Fetch targets and print 'em */
	do {
		last_target = target;

		if (!strncmp(str, target->name, strlen(str)) &&
		    /* dummy prereq on multipath version */
		    target->version[0] >= x &&
		    target->version[1] >= y &&
		    target->version[2] >= z
		   )
			r = 0;

		target = (void *) target + target->next;
	} while (last_target != target);

	out:
	dm_task_destroy(dmt);
	return r;
}
