static unsigned int st_readmap(struct autofs_point *ap)
{
	pthread_t thid;
	struct readmap_args *ra;
	int status;
	int now = time(NULL);

	debug(ap->logopt, "state %d path %s", ap->state, ap->path);

	assert(ap->state == ST_READY);
	assert(ap->readmap_thread == 0);

	ap->state = ST_READMAP;

	ra = malloc(sizeof(struct readmap_args));
	if (!ra) {
		error(ap->logopt, "failed to malloc reamap cond struct");
		/* It didn't work: return to ready */
		st_ready(ap);
		if (!ap->submount)
			alarm_add(ap, ap->exp_runfreq);
		return 0;
	}

	status = pthread_mutex_init(&ra->mutex, NULL);
	if (status)
		fatal(status);

	status = pthread_cond_init(&ra->cond, NULL);
	if (status)
		fatal(status);

	status = pthread_mutex_lock(&ra->mutex);
	if (status)
		fatal(status);

	ra->ap = ap;
	ra->now = now;

	status = pthread_create(&thid, &th_attr_detached, do_readmap, ra);
	if (status) {
		error(ap->logopt, "read map thread create failed");
		st_readmap_cleanup(ra);
		free(ra);
		/* It didn't work: return to ready */
		st_ready(ap);
		if (!ap->submount)
			alarm_add(ap, ap->exp_runfreq);
		return 0;
	}
	ap->readmap_thread = thid;
	st_set_thid(ap, thid);

	pthread_cleanup_push(st_readmap_cleanup, ra);

	ra->signaled = 0;
	while (!ra->signaled) {
		status = pthread_cond_wait(&ra->cond, &ra->mutex);
		if (status)
			fatal(status);
	}

	pthread_cleanup_pop(1);

	return 1;
}