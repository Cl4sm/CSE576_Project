void __cl_timer_prov_destroy(void)
{
	pthread_t tid;

	if (!gp_timer_prov)
		return;

	tid = gp_timer_prov->thread;
	pthread_mutex_lock(&gp_timer_prov->mutex);
	gp_timer_prov->exit = TRUE;
	pthread_cond_broadcast(&gp_timer_prov->cond);
	pthread_mutex_unlock(&gp_timer_prov->mutex);
	pthread_join(tid, NULL);

	/* Destroy the mutex and condition variable. */
	pthread_mutex_destroy(&gp_timer_prov->mutex);
	pthread_cond_destroy(&gp_timer_prov->cond);

	/* Free the memory and reset the global pointer. */
	free(gp_timer_prov);
	gp_timer_prov = NULL;
}