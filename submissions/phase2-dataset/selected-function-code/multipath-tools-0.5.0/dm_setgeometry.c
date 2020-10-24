{
	struct dm_task *dmt;
	struct path *pp;
	char heads[4], sectors[4];
	char cylinders[10], start[32];
	int r = 0;

	if (!mpp)
		return 1;

	pp = first_path(mpp);
	if (!pp) {
		condlog(3, "%s: no path for geometry", mpp->alias);
		return 1;
	}
	if (pp->geom.cylinders == 0 ||
	    pp->geom.heads == 0 ||
	    pp->geom.sectors == 0) {
		condlog(3, "%s: invalid geometry on %s", mpp->alias, pp->dev);
		return 1;
	}

	if (!(dmt = dm_task_create(DM_DEVICE_SET_GEOMETRY)))
		return 0;

	if (!dm_task_set_name(dmt, mpp->alias))
		goto out;

	dm_task_no_open_count(dmt);

	/* What a sick interface ... */
	snprintf(heads, 4, "%u", pp->geom.heads);
	snprintf(sectors, 4, "%u", pp->geom.sectors);
	snprintf(cylinders, 10, "%u", pp->geom.cylinders);
	snprintf(start, 32, "%lu", pp->geom.start);
	if (!dm_task_set_geometry(dmt, cylinders, heads, sectors, start)) {
		condlog(3, "%s: Failed to set geometry", mpp->alias);
		goto out;
	}

	r = dm_task_run(dmt);
out:
	dm_task_destroy(dmt);

	return r;
}
