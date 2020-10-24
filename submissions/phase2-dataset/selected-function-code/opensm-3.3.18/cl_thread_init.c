cl_status_t cl_thread_init(IN cl_thread_t * const p_thread,
			   IN cl_pfn_thread_callback_t pfn_callback,
			   IN const void *const context,
			   IN const char *const name)
{
	int ret;

	CL_ASSERT(p_thread);

	cl_thread_construct(p_thread);

	/* Initialize the thread structure */
	p_thread->pfn_callback = pfn_callback;
	p_thread->context = context;

	ret = pthread_create(&p_thread->osd.id, NULL,
			     __cl_thread_wrapper, (void *)p_thread);

	if (ret != 0)		/* pthread_create returns a "0" for success */
		return (CL_ERROR);

	p_thread->osd.state = CL_INITIALIZED;

	return (CL_SUCCESS);
}