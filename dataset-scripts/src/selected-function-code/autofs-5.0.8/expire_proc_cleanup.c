void expire_proc_cleanup(void *arg)
{
	struct expire_args *ea;
	int status;

	ea = (struct expire_args *) arg;

	status = pthread_mutex_unlock(&ea->mutex);
	if (status)
		fatal(status);

	status = pthread_cond_destroy(&ea->cond);
	if (status)
		fatal(status);

	status = pthread_mutex_destroy(&ea->mutex);
	if (status)
		fatal(status);

	free(ea);

	return;
}