int
dm_geteventnr (char *name)
{
	struct dm_task *dmt;
	struct dm_info info;
	int event = -1;

	if (!(dmt = dm_task_create(DM_DEVICE_INFO)))
		return -1;

	if (!dm_task_set_name(dmt, name))
		goto out;

	dm_task_no_open_count(dmt);

	if (!dm_task_run(dmt))
		goto out;

	if (!dm_task_get_info(dmt, &info))
		goto out;

	if (info.exists)
		event = info.event_nr;

out:
	dm_task_destroy(dmt);

	return event;
}
