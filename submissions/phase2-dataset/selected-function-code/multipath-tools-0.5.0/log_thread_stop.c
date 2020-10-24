{
	logdbg(stderr,"enter log_thread_stop\n");

	pthread_mutex_lock(&logev_lock);
	logq_running = 0;
	pthread_cond_signal(&logev_cond);
	pthread_mutex_unlock(&logev_lock);

	pthread_mutex_lock(&logq_lock);
	pthread_cancel(log_thr);
	pthread_mutex_unlock(&logq_lock);
	pthread_join(log_thr, NULL);
	log_thr = (pthread_t)0;

	flush_logqueue();

	pthread_mutex_destroy(&logq_lock);
	pthread_mutex_destroy(&logev_lock);
	pthread_cond_destroy(&logev_cond);

	log_close();
}
