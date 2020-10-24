int dm_reassign_table(const char *name, char *old, char *new)
{
	int r, modified = 0;
	uint64_t start, length;
	struct dm_task *dmt, *reload_dmt;
	char *target, *params = NULL;
	char buff[PARAMS_SIZE];
	void *next = NULL;

	if (!(dmt = dm_task_create(DM_DEVICE_TABLE)))
		return 0;

	if (!dm_task_set_name(dmt, name))
		goto out;

	dm_task_no_open_count(dmt);

	if (!dm_task_run(dmt))
		goto out;
	if (!(reload_dmt = dm_task_create(DM_DEVICE_RELOAD)))
		goto out;
	if (!dm_task_set_name(reload_dmt, name))
		goto out_reload;

	do {
		next = dm_get_next_target(dmt, next, &start, &length,
					  &target, &params);
		memset(buff, 0, PARAMS_SIZE);
		strcpy(buff, params);
		if (strcmp(target, TGT_MPATH) && strstr(params, old)) {
			condlog(3, "%s: replace target %s %s",
				name, target, buff);
			dm_reassign_deps(buff, old, new);
			condlog(3, "%s: with target %s %s",
				name, target, buff);
			modified++;
		}
		dm_task_add_target(reload_dmt, start, length, target, buff);
	} while (next);

	if (modified) {
		dm_task_no_open_count(reload_dmt);

		if (!dm_task_run(reload_dmt)) {
			condlog(3, "%s: failed to reassign targets", name);
			goto out_reload;
		}
		dm_simplecmd_noflush(DM_DEVICE_RESUME, name);
	}
	r = 1;

out_reload:
	dm_task_destroy(reload_dmt);
out:
	dm_task_destroy(dmt);
	return r;
}
