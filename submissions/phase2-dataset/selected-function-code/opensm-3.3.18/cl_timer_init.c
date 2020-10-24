cl_status_t cl_timer_init(IN cl_timer_t * const p_timer,
			  IN cl_pfn_timer_callback_t pfn_callback,
			  IN const void *const context)
{
	CL_ASSERT(p_timer);
	CL_ASSERT(pfn_callback);

	cl_timer_construct(p_timer);

	if (!gp_timer_prov)
		return (CL_ERROR);

	/* Store timer parameters. */
	p_timer->pfn_callback = pfn_callback;
	p_timer->context = context;

	/* Mark the timer as idle. */
	p_timer->timer_state = CL_TIMER_IDLE;

	/* Create the condition variable that is used when cancelling a timer. */
	pthread_cond_init(&p_timer->cond, NULL);

	p_timer->state = CL_INITIALIZED;

	return (CL_SUCCESS);
}