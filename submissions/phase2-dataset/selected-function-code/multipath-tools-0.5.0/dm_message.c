dm_message(char * mapname, char * message)
{
	int r = 1;
	struct dm_task *dmt;

	if (!(dmt = dm_task_create(DM_DEVICE_TARGET_MSG)))
		return 1;

	if (!dm_task_set_name(dmt, mapname))
		goto out;

	if (!dm_task_set_sector(dmt, 0))
		goto out;

	if (!dm_task_set_message(dmt, message))
		goto out;

	dm_task_no_open_count(dmt);

	if (!dm_task_run(dmt))
		goto out;

	r = 0;
out:
	if (r)
		condlog(0, "DM message failed [%s]", message);

	dm_task_destroy(dmt);
	return r;
}
