int handle_mounts_startup_cond_init(struct startup_cond *suc)
{
	int status;

	status = pthread_mutex_init(&suc->mutex, NULL);
	if (status)
		return status;

	status = pthread_cond_init(&suc->cond, NULL);
	if (status) {
		status = pthread_mutex_destroy(&suc->mutex);
		if (status)
			fatal(status);
		return status;
	}

	status = pthread_mutex_lock(&suc->mutex);
	if (status) {
		status = pthread_mutex_destroy(&suc->mutex);
		if (status)
			fatal(status);
		status = pthread_cond_destroy(&suc->cond);
		if (status)
			fatal(status);
	}

	return 0;
}