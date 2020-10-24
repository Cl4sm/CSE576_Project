cl_status_t cl_thread_pool_init(IN cl_thread_pool_t * const p_thread_pool,
				IN unsigned count,
				IN void (*pfn_callback) (void *),
				IN void *context, IN const char *const name)
{
	int i;

	CL_ASSERT(p_thread_pool);
	CL_ASSERT(pfn_callback);

	memset(p_thread_pool, 0, sizeof(*p_thread_pool));

	if (!count)
		count = cl_proc_count();

	pthread_mutex_init(&p_thread_pool->mutex, NULL);
	pthread_cond_init(&p_thread_pool->cond, NULL);

	p_thread_pool->events = 0;

	p_thread_pool->pfn_callback = pfn_callback;
	p_thread_pool->context = context;

	p_thread_pool->tid = calloc(count, sizeof(*p_thread_pool->tid));
	if (!p_thread_pool->tid) {
		cl_thread_pool_destroy(p_thread_pool);
		return CL_INSUFFICIENT_MEMORY;
	}

	p_thread_pool->running_count = count;

	for (i = 0; i < count; i++) {
		if (pthread_create(&p_thread_pool->tid[i], NULL,
				   thread_pool_routine, p_thread_pool) != 0) {
			cl_thread_pool_destroy(p_thread_pool);
			return CL_INSUFFICIENT_RESOURCES;
		}
	}

	return (CL_SUCCESS);
}