load_config (char * file, struct udev *udev)
{
	if (!conf)
		conf = alloc_config();

	if (!conf || !udev)
		return 1;

	/*
	 * internal defaults
	 */
	if (!conf->verbosity)
		conf->verbosity = DEFAULT_VERBOSITY;

	conf->udev = udev;
	conf->dev_type = DEV_NONE;
	conf->minio = DEFAULT_MINIO;
	conf->minio_rq = DEFAULT_MINIO_RQ;
	get_sys_max_fds(&conf->max_fds);
	conf->bindings_file = set_default(DEFAULT_BINDINGS_FILE);
	conf->wwids_file = set_default(DEFAULT_WWIDS_FILE);
	conf->bindings_read_only = 0;
	conf->multipath_dir = set_default(DEFAULT_MULTIPATHDIR);
	conf->features = set_default(DEFAULT_FEATURES);
	conf->flush_on_last_del = 0;
	conf->attribute_flags = 0;
	conf->reassign_maps = DEFAULT_REASSIGN_MAPS;
	conf->checkint = DEFAULT_CHECKINT;
	conf->max_checkint = MAX_CHECKINT(conf->checkint);
	conf->pgfailback = DEFAULT_FAILBACK;
	conf->fast_io_fail = DEFAULT_FAST_IO_FAIL;
	conf->retain_hwhandler = DEFAULT_RETAIN_HWHANDLER;
	conf->detect_prio = DEFAULT_DETECT_PRIO;

	/*
	 * preload default hwtable
	 */
	if (conf->hwtable == NULL) {
		conf->hwtable = vector_alloc();

		if (!conf->hwtable)
			goto out;
	}
	if (setup_default_hwtable(conf->hwtable))
		goto out;

	/*
	 * read the config file
	 */
	set_current_keywords(&conf->keywords);
	alloc_keywords();
	if (filepresent(file)) {
		int builtin_hwtable_size;

		builtin_hwtable_size = VECTOR_SIZE(conf->hwtable);
		if (init_data(file, init_keywords)) {
			condlog(0, "error parsing config file");
			goto out;
		}
		if (VECTOR_SIZE(conf->hwtable) > builtin_hwtable_size) {
			/*
			 * remove duplica in hwtable. config file
			 * takes precedence over build-in hwtable
			 */
			factorize_hwtable(conf->hwtable, builtin_hwtable_size);
		}

	} else {
		init_keywords();
	}

	/*
	 * fill the voids left in the config file
	 */
	if (conf->blist_devnode == NULL) {
		conf->blist_devnode = vector_alloc();

		if (!conf->blist_devnode)
			goto out;
	}
	if (conf->blist_wwid == NULL) {
		conf->blist_wwid = vector_alloc();

		if (!conf->blist_wwid)
			goto out;
	}
	if (conf->blist_device == NULL) {
		conf->blist_device = vector_alloc();

		if (!conf->blist_device)
			goto out;
	}
	if (conf->blist_property == NULL) {
		conf->blist_property = vector_alloc();

		if (!conf->blist_property)
			goto out;
	}

	if (conf->elist_devnode == NULL) {
		conf->elist_devnode = vector_alloc();

		if (!conf->elist_devnode)
			goto out;
	}
	if (conf->elist_wwid == NULL) {
		conf->elist_wwid = vector_alloc();

		if (!conf->elist_wwid)
			goto out;
	}

	if (conf->elist_device == NULL) {
		conf->elist_device = vector_alloc();

		if (!conf->elist_device)
			goto out;
	}

	if (conf->elist_property == NULL) {
		conf->elist_property = vector_alloc();

		if (!conf->elist_property)
			goto out;
	}
	if (setup_default_blist(conf))
		goto out;

	if (conf->mptable == NULL) {
		conf->mptable = vector_alloc();
		if (!conf->mptable)
			goto out;
	}
	if (conf->bindings_file == NULL)
		conf->bindings_file = set_default(DEFAULT_BINDINGS_FILE);

	if (!conf->multipath_dir || !conf->bindings_file ||
	    !conf->wwids_file)
		goto out;

	return 0;
out:
	free_config(conf);
	return 1;
}
