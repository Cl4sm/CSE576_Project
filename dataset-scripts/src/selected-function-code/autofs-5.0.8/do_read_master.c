static void *do_read_master(void *arg)
{
	struct master *master;
	unsigned int logopt;
	time_t age;
	int readall = 1;
	int status;

	status = pthread_mutex_lock(&mrc.mutex);
	if (status)
		fatal(status);

	master = mrc.master;
	age = mrc.age;
	logopt = master->logopt;

	mrc.signaled = 1;
	status = pthread_cond_signal(&mrc.cond);
	if (status) {
		error(logopt,
		      "failed to signal master read map condition");
		master->reading = 0;
		status = pthread_mutex_unlock(&mrc.mutex);
		if (status)
			fatal(status);
		pthread_exit(NULL);
	}

	status = pthread_mutex_unlock(&mrc.mutex);
	if (status)
		fatal(status);

	defaults_read_config(1);

	info(logopt, "re-reading master map %s", master->name);

	status = master_read_master(master, age, readall);

	master->reading = 0;

	return NULL;
}