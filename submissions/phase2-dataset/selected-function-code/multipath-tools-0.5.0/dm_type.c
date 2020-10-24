dm_type(const char * name, char * type)
{
	int r = 0;
	struct dm_task *dmt;
	void *next = NULL;
	uint64_t start, length;
	char *target_type = NULL;
	char *params;

	if (!(dmt = dm_task_create(DM_DEVICE_TABLE)))
		return 0;

	if (!dm_task_set_name(dmt, name))
		goto out;

	dm_task_no_open_count(dmt);

	if (!dm_task_run(dmt))
		goto out;

	/* Fetch 1st target */
	next = dm_get_next_target(dmt, next, &start, &length,
				  &target_type, &params);

	if (!target_type)
		r = -1;
	else if (!strcmp(target_type, type))
		r = 1;

out:
	dm_task_destroy(dmt);
	return r;
}
