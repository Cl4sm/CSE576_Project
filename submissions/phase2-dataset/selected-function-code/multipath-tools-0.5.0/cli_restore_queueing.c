int
cli_restore_queueing(void *v, char **reply, int *len, void *data)
{
	struct vectors * vecs = (struct vectors *)data;
	char * mapname = get_keyparam(v, MAP);
	struct multipath *mpp;
	int minor;

	mapname = convert_dev(mapname, 0);
	condlog(2, "%s: restore map queueing (operator)", mapname);
	if (sscanf(mapname, "dm-%d", &minor) == 1)
		mpp = find_mp_by_minor(vecs->mpvec, minor);
	else
		mpp = find_mp_by_alias(vecs->mpvec, mapname);

	if (!mpp) {
		condlog(0, "%s: invalid map name, cannot restore queueing", mapname);
		return 1;
	}

	if (mpp->no_path_retry != NO_PATH_RETRY_UNDEF &&
			mpp->no_path_retry != NO_PATH_RETRY_FAIL) {
		dm_queue_if_no_path(mpp->alias, 1);
		if (mpp->nr_active > 0)
			mpp->retry_tick = 0;
		else
			mpp->retry_tick = mpp->no_path_retry * conf->checkint;
	}
	return 0;
}
