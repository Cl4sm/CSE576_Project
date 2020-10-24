static enum expire expire_proc(struct autofs_point *ap, int now)
{
	pthread_t thid;
	struct expire_args *ea;
	void *(*expire)(void *);
	int status;

	assert(ap->exp_thread == 0);

	ea = malloc(sizeof(struct expire_args));
	if (!ea) {
		error(ap->logopt, "failed to malloc expire cond struct");
		return EXP_ERROR;
	}

	status = pthread_mutex_init(&ea->mutex, NULL);
	if (status)
		fatal(status);

	status = pthread_cond_init(&ea->cond, NULL);
	if (status)
		fatal(status);

	status = pthread_mutex_lock(&ea->mutex);
	if (status)
		fatal(status);

	ea->ap = ap;
	ea->when = now;
	ea->status = 1;

	if (ap->type == LKP_INDIRECT)
		expire = expire_proc_indirect;
	else
		expire = expire_proc_direct;

	status = pthread_create(&thid, &th_attr_detached, expire, ea);
	if (status) {
		error(ap->logopt,
		      "expire thread create for %s failed", ap->path);
		expire_proc_cleanup((void *) ea);
		return EXP_ERROR;
	}
	ap->exp_thread = thid;
	st_set_thid(ap, thid);

	pthread_cleanup_push(expire_proc_cleanup, ea);

	debug(ap->logopt, "exp_proc = %lu path %s",
		(unsigned long) ap->exp_thread, ap->path);

	ea->signaled = 0;
	while (!ea->signaled) {
		status = pthread_cond_wait(&ea->cond, &ea->mutex);
		if (status)
			fatal(status);
	}

	pthread_cleanup_pop(1);

	return EXP_STARTED;
}