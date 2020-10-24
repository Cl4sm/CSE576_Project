static void *do_notify_state(void *arg)
{
	struct master *master;
	int sig;
	int status;

	sig = *(int *) arg;

	status = pthread_mutex_lock(&mrc.mutex);
	if (status)
		fatal(status);

	master = mrc.master;

	debug(master->logopt, "signal %d", sig);

	mrc.signaled = 1;
	status = pthread_cond_signal(&mrc.cond);
	if (status) {
		error(master->logopt,
		      "failed to signal state notify condition");
		status = pthread_mutex_unlock(&mrc.mutex);
		if (status)
			fatal(status);
		pthread_exit(NULL);
	}

	status = pthread_mutex_unlock(&mrc.mutex);
	if (status)
		fatal(status);

	master_notify_state_change(master, sig);

	return NULL;
}