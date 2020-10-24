int
dm_rename_partmaps (char * old, char * new)
{
	struct dm_task *dmt;
	struct dm_names *names;
	unsigned next = 0;
	char buff[PARAMS_SIZE];
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

	if (dm_dev_t(old, &dev_t[0], 32))
		goto out;

	do {
		if (
		    /*
		     * if devmap target is "linear"
		     */
		    (dm_type(names->name, TGT_PART) > 0) &&

		    /*
		     * and the multipath mapname and the part mapname start
		     * the same
		     */
		    !strncmp(names->name, old, strlen(old)) &&

		    /*
		     * and we can fetch the map table from the kernel
		     */
		    !dm_get_map(names->name, &size, &buff[0]) &&

		    /*
		     * and the table maps over the multipath map
		     */
		    strstr(buff, dev_t)
		   ) {
				/*
				 * then it's a kpartx generated partition.
				 * Rename it.
				 */
				snprintf(buff, PARAMS_SIZE, "%s%s",
					 new, names->name + strlen(old));
				dm_rename(names->name, buff);
				condlog(4, "partition map %s renamed",
					names->name);
		   }

		next = names->next;
		names = (void *) names + next;
	} while (next);

	r = 0;
out:
	dm_task_destroy (dmt);
	return r;
}
