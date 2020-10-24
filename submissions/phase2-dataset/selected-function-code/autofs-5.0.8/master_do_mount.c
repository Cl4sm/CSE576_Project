static int master_do_mount(struct master_mapent *entry)
{
	struct startup_cond suc;
	struct autofs_point *ap;
	pthread_t thid;
	int status;

	ap = entry->ap;

	if (handle_mounts_startup_cond_init(&suc)) {
		crit(ap->logopt,
		     "failed to init startup cond for mount %s", entry->path);
		return 0;
	}

	suc.ap = ap;
	suc.root = ap->path;
	suc.done = 0;
	suc.status = 0;

	debug(ap->logopt, "mounting %s", entry->path);

	status = pthread_create(&thid, &th_attr, handle_mounts, &suc);
	if (status) {
		crit(ap->logopt,
		     "failed to create mount handler thread for %s",
		     entry->path);
		handle_mounts_startup_cond_destroy(&suc);
		return 0;
	}

	while (!suc.done) {
		status = pthread_cond_wait(&suc.cond, &suc.mutex);
		if (status)
			fatal(status);
	}

	if (suc.status) {
		error(ap->logopt, "failed to startup mount");
		handle_mounts_startup_cond_destroy(&suc);
		return 0;
	}
	entry->thid = thid;

	handle_mounts_startup_cond_destroy(&suc);

	return 1;
}