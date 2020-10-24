dm_remove_partmaps (const char * mapname, int need_sync)
{
	struct dm_task *dmt;
	struct dm_names *names;
	unsigned next = 0;
	char params[PARAMS_SIZE];
	unsigned long long size;
	char dev_t[32];
	int r = 1;

	if (!(dmt = dm_task_create(DM_DEVICE_LIST)))
		return 1;

	dm_task_no_open_count(dmt);

	if (!dm_task_run(dmt))
		goto out;

	if (!(names = dm_task_get_names(dmt)))
		goto out;

	if (!names->dev) {
		r = 0; /* this is perfectly valid */
		goto out;
	}

	if (dm_dev_t(mapname, &dev_t[0], 32))
		goto out;

	do {
		if (
		    /*
		     * if devmap target is "linear"
		     */
		    (dm_type(names->name, TGT_PART) > 0) &&

		    /*
		     * and both uuid end with same suffix starting
		     * at UUID_PREFIX
		     */
		    (!dm_compare_uuid(names->name, mapname)) &&

		    /*
		     * and we can fetch the map table from the kernel
		     */
		    !dm_get_map(names->name, &size, &params[0]) &&

		    /*
		     * and the table maps over the multipath map
		     */
		    strstr(params, dev_t)
		   ) {
			/*
			 * then it's a kpartx generated partition.
			 * remove it.
			 */
			/*
			 * if the opencount is 0 maybe some other
			 * partitions depend on it.
			 */
			if (dm_get_opencount(names->name)) {
				dm_remove_partmaps(names->name, need_sync);
				if (dm_get_opencount(names->name)) {
					condlog(2, "%s: map in use",
						names->name);
					goto out;
				}
			}
			condlog(4, "partition map %s removed",
				names->name);
			dm_simplecmd_flush(DM_DEVICE_REMOVE, names->name,
					   need_sync);
		}

		next = names->next;
		names = (void *) names + next;
	} while (next);

	r = 0;
out:
	dm_task_destroy (dmt);
	return r;
}
