int
dm_get_info (char * mapname, struct dm_info ** dmi)
{
	int r = 1;
	struct dm_task *dmt = NULL;

	if (!mapname)
		return 1;

	if (!*dmi)
		*dmi = alloc_dminfo();

	if (!*dmi)
		return 1;

	if (!(dmt = dm_task_create(DM_DEVICE_INFO)))
		goto out;

	if (!dm_task_set_name(dmt, mapname))
		goto out;

	dm_task_no_open_count(dmt);

	if (!dm_task_run(dmt))
		goto out;

	if (!dm_task_get_info(dmt, *dmi))
		goto out;

	r = 0;
out:
	if (r) {
		memset(*dmi, 0, sizeof(struct dm_info));
		FREE(*dmi);
		*dmi = NULL;
	}

	if (dmt)
		dm_task_destroy(dmt);

	return r;
}
