static int
dm_get_prefixed_uuid(const char *name, char *uuid)
{
	struct dm_task *dmt;
	const char *uuidtmp;
	int r = 1;

	dmt = dm_task_create(DM_DEVICE_INFO);
	if (!dmt)
		return 1;

	if (!dm_task_set_name (dmt, name))
		goto uuidout;

	if (!dm_task_run(dmt))
		goto uuidout;

	uuidtmp = dm_task_get_uuid(dmt);
	if (uuidtmp)
		strcpy(uuid, uuidtmp);
	else
		uuid[0] = '\0';

	r = 0;
uuidout:
	dm_task_destroy(dmt);
	return r;
}
