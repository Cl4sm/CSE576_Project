static void handle_mounts_cleanup(void *arg)
{
	struct autofs_point *ap;
	char path[PATH_MAX + 1];
	char buf[MAX_ERR_BUF];
	unsigned int clean = 0, submount, logopt;
	unsigned int pending = 0;

	ap = (struct autofs_point *) arg;

	logopt = ap->logopt;
	submount = ap->submount;

	strcpy(path, ap->path);
	if (!submount && strcmp(ap->path, "/-") &&
	    ap->flags & MOUNT_FLAG_DIR_CREATED)
		clean = 1;

	if (submount) {
		/* We are finishing up */
		ap->parent->submnt_count--;
		list_del_init(&ap->mounts);
	}

	/* Don't signal the handler if we have already done so */
	if (!list_empty(&master_list->completed))
		pending = 1;
	master_remove_mapent(ap->entry);
	master_source_unlock(ap->entry);

	destroy_logpri_fifo(ap);

	/*
	 * Submounts are detached threads and don't belong to the
	 * master map entry list so we need to free their resources
	 * here.
	 */
	if (submount) {
		mounts_mutex_unlock(ap->parent);
		master_source_unlock(ap->parent->entry);
		master_free_mapent_sources(ap->entry, 1);
		master_free_mapent(ap->entry);
	}

	if (clean) {
		if (rmdir(path) == -1) {
			char *estr = strerror_r(errno, buf, MAX_ERR_BUF);
			warn(logopt, "failed to remove dir %s: %s",
			     path, estr);
		}
	}

	info(logopt, "shut down path %s", path);

	/*
	 * If we are not a submount send a signal to the signal handler
	 * so it can join with any completed handle_mounts() threads and
	 * perform final cleanup.
	 */
	if (!submount && !pending)
		pthread_kill(state_mach_thid, SIGTERM);

	master_mutex_unlock();

	return;
}