static void st_readmap_cleanup(void *arg)
{
	struct readmap_args *ra;
	int status;

	ra = (struct readmap_args *) arg;

	status = pthread_mutex_unlock(&ra->mutex);
	if (status)
		fatal(status);

	status = pthread_cond_destroy(&ra->cond);
	if (status)
		fatal(status);

	status = pthread_mutex_destroy(&ra->mutex);
	if (status)
		fatal(status);

	return;
}