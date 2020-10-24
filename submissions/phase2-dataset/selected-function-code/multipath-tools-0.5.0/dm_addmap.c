extern int
dm_addmap (int task, const char *name, const char *target,
	   const char *params, uint64_t size, int ro, const char *uuid, int part,
	   mode_t mode, uid_t uid, gid_t gid, uint32_t *cookie) {
	int r = 0;
	struct dm_task *dmt;
	char *prefixed_uuid = NULL;

	if (!(dmt = dm_task_create (task)))
		return 0;

	if (!dm_task_set_name (dmt, name))
		goto addout;

	if (!dm_task_add_target (dmt, 0, size, target, params))
		goto addout;

	if (ro && !dm_task_set_ro (dmt))
			goto addout;

	if (task == DM_DEVICE_CREATE && uuid) {
		prefixed_uuid = malloc(MAX_PREFIX_LEN + strlen(uuid) + 1);
		if (!prefixed_uuid) {
			fprintf(stderr, "cannot create prefixed uuid : %s\n",
				strerror(errno));
			goto addout;
		}
		sprintf(prefixed_uuid, UUID_PREFIX "%s", part, uuid);
		if (!dm_task_set_uuid(dmt, prefixed_uuid))
			goto addout;
	}

	if (!dm_task_set_mode(dmt, mode))
		goto addout;
	if (!dm_task_set_uid(dmt, uid))
		goto addout;
	if (!dm_task_set_gid(dmt, gid))
		goto addout;

	dm_task_no_open_count(dmt);

	if (task == DM_DEVICE_CREATE && !dm_task_set_cookie(dmt, cookie, (udev_sync)? 0 : DM_UDEV_DISABLE_LIBRARY_FALLBACK))
		goto addout;
	r = dm_task_run (dmt);

	addout:
	dm_task_destroy (dmt);

	return r;
}
