void cl_thread_pool_destroy(IN cl_thread_pool_t * const p_thread_pool)
{
	int i;

	CL_ASSERT(p_thread_pool);

	for (i = 0; i < p_thread_pool->running_count; i++)
		if (p_thread_pool->tid[i])
			pthread_cancel(p_thread_pool->tid[i]);

	for (i = 0; i < p_thread_pool->running_count; i++)
		if (p_thread_pool->tid[i])
			pthread_join(p_thread_pool->tid[i], NULL);

	p_thread_pool->running_count = 0;

	free(p_thread_pool->tid);

	pthread_cond_destroy(&p_thread_pool->cond);
	pthread_mutex_destroy(&p_thread_pool->mutex);

	p_thread_pool->events = 0;
}