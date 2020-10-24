static void * log_thread (void * et)
{
	int running;

	pthread_mutex_lock(&logev_lock);
	logq_running = 1;
	pthread_mutex_unlock(&logev_lock);

	mlockall(MCL_CURRENT | MCL_FUTURE);
	logdbg(stderr,"enter log_thread\n");

	while (1) {
		pthread_mutex_lock(&logev_lock);
		pthread_cond_wait(&logev_cond, &logev_lock);
		running = logq_running;
		pthread_mutex_unlock(&logev_lock);
		if (!running)
			break;
		log_thread_flush();
	}
	return NULL;
}
