static pthread_t do_signals(struct master *master, int sig)
{
	pthread_t thid;
	int r_sig = sig;
	int status;

	status = pthread_mutex_lock(&mrc.mutex);
	if (status)
		fatal(status);

	status = pthread_create(&thid, &th_attr_detached, do_notify_state, &r_sig);
	if (status) {
		error(master->logopt,
		      "mount state notify thread create failed");
		status = pthread_mutex_unlock(&mrc.mutex);
		if (status)
			fatal(status);
		return 0;
	}

	mrc.thid = thid;
	mrc.master = master;

	pthread_cleanup_push(do_master_cleanup_unlock, NULL);

	mrc.signaled = 0;
	while (!mrc.signaled) {
		status = pthread_cond_wait(&mrc.cond, &mrc.mutex);
		if (status)
			fatal(status);
	}

	pthread_cleanup_pop(1);

	return thid;
}