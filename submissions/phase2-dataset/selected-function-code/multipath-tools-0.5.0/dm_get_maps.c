dm_get_maps (vector mp)
{
	struct multipath * mpp;
	int r = 1;
	int info;
	struct dm_task *dmt;
	struct dm_names *names;
	unsigned next = 0;

	if (!mp)
		return 1;

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

	do {
		info = dm_type(names->name, TGT_MPATH);

		if (info <= 0)
			goto next;

		mpp = alloc_multipath();

		if (!mpp)
			goto out;

		mpp->alias = STRDUP(names->name);

		if (!mpp->alias)
			goto out1;

		if (info > 0) {
			if (dm_get_map(names->name, &mpp->size, NULL))
				goto out1;

			dm_get_uuid(names->name, mpp->wwid);
			dm_get_info(names->name, &mpp->dmi);
		}

		if (!vector_alloc_slot(mp))
			goto out1;

		vector_set_slot(mp, mpp);
		mpp = NULL;
next:
		next = names->next;
		names = (void *) names + next;
	} while (next);

	r = 0;
	goto out;
out1:
	free_multipath(mpp, KEEP_PATHS);
out:
	dm_task_destroy (dmt);
	return r;
}
