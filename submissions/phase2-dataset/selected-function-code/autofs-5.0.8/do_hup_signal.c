static int do_hup_signal(struct master *master, time_t age)
{
	unsigned int logopt = master->logopt;
	pthread_t thid;
	int status;

	status = pthread_mutex_lock(&mrc.mutex);
	if (status)
		fatal(status);

	master_mutex_lock();
	/* Already doing a map read or shutdown or no mounts */
	if (master->reading) {
		status = pthread_mutex_unlock(&mrc.mutex);
		if (status)
			fatal(status);
		master_mutex_unlock();
		return 1;
	}
	master->reading = 1;
	master_mutex_unlock();

	status = pthread_create(&thid, &th_attr_detached, do_read_master, NULL);
	if (status) {
		error(logopt,
		      "master read map thread create failed");
		master->reading = 0;
		status = pthread_mutex_unlock(&mrc.mutex);
		if (status)
			fatal(status);
		return 0;
	}

	mrc.thid = thid;
	mrc.master = master;
	mrc.age = age;

	pthread_cleanup_push(do_master_cleanup_unlock, NULL);

	mrc.signaled = 0;
	while (!mrc.signaled) {
		status = pthread_cond_wait(&mrc.cond, &mrc.mutex);
		if (status)
			fatal(status);
	}

	pthread_cleanup_pop(1);

	return 1;
}