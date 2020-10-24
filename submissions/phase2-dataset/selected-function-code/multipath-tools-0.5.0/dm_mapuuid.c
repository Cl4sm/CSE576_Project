dm_mapuuid(int major, int minor)
{
	struct dm_task *dmt;
	const char *tmp;
	char *uuid = NULL;

	if (!(dmt = dm_task_create(DM_DEVICE_INFO)))
		return NULL;

	dm_task_no_open_count(dmt);
	dm_task_set_major(dmt, major);
	dm_task_set_minor(dmt, minor);

	if (!dm_task_run(dmt))
		goto out;

	tmp = dm_task_get_uuid(dmt);
	if (tmp[0] != '\0')
		uuid = strdup(tmp);
out:
	dm_task_destroy(dmt);
	return uuid;
}
