int
reconfigure (struct vectors * vecs)
{
	struct config * old = conf;
	int retval = 1;

	/*
	 * free old map and path vectors ... they use old conf state
	 */
	if (VECTOR_SIZE(vecs->mpvec))
		remove_maps_and_stop_waiters(vecs);

	if (VECTOR_SIZE(vecs->pathvec))
		free_pathvec(vecs->pathvec, FREE_PATHS);

	vecs->pathvec = NULL;
	conf = NULL;

	/* Re-read any timezone changes */
	tzset();

	if (!load_config(DEFAULT_CONFIGFILE, udev)) {
		dm_drv_version(conf->version, TGT_MPATH);
		conf->verbosity = old->verbosity;
		conf->daemon = 1;
		configure(vecs, 1);
		free_config(old);
		retval = 0;
	}

	return retval;
}
