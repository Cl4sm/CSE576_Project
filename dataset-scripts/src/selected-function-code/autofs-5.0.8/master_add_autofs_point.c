int master_add_autofs_point(struct master_mapent *entry, unsigned logopt,
			    unsigned nobind, unsigned ghost, int submount)
{
	struct autofs_point *ap;
	int status;

	ap = malloc(sizeof(struct autofs_point));
	if (!ap)
		return 0;

	ap->state = ST_INIT;

	ap->state_pipe[0] = -1;
	ap->state_pipe[1] = -1;
	ap->logpri_fifo = -1;

	ap->path = strdup(entry->path);
	if (!ap->path) {
		free(ap);
		return 0;
	}

	ap->entry = entry;
	ap->exp_thread = 0;
	ap->readmap_thread = 0;
	/*
	 * Program command line option overrides config.
	 * We can't use 0 negative timeout so use default.
	 */
	if (global_negative_timeout <= 0)
		ap->negative_timeout = defaults_get_negative_timeout();
	else
		ap->negative_timeout = global_negative_timeout;
	ap->exp_runfreq = 0;
	ap->flags = 0;
	if (ghost)
		ap->flags = MOUNT_FLAG_GHOST;

	if (nobind)
		ap->flags |= MOUNT_FLAG_NOBIND;

	if (ap->path[1] == '-')
		ap->type = LKP_DIRECT;
	else
		ap->type = LKP_INDIRECT;

	ap->logopt = logopt;

	ap->parent = NULL;
	ap->thid = 0;
	ap->submnt_count = 0;
	ap->submount = submount;
	INIT_LIST_HEAD(&ap->mounts);
	INIT_LIST_HEAD(&ap->submounts);
	ap->shutdown = 0;

	status = pthread_mutex_init(&ap->mounts_mutex, NULL);
	if (status) {
		free(ap->path);
		free(ap);
		return 0;
	}

	entry->ap = ap;

	return 1;
}