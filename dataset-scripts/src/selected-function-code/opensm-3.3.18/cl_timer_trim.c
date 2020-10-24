cl_status_t cl_timer_trim(IN cl_timer_t * const p_timer,
			  IN const uint32_t time_ms)
{
	struct timespec newtime;
	cl_status_t status;

	CL_ASSERT(p_timer);
	CL_ASSERT(p_timer->state == CL_INITIALIZED);

	pthread_mutex_lock(&gp_timer_prov->mutex);

	__cl_timer_calculate(time_ms, &newtime);

	if (p_timer->timer_state == CL_TIMER_QUEUED) {
		/* If the old time is earlier, do not trim it.  Just return. */
		if (__cl_timer_is_earlier(&p_timer->timeout, &newtime)) {
			pthread_mutex_unlock(&gp_timer_prov->mutex);
			return (CL_SUCCESS);
		}
	}

	/* Reset the timer to the new timeout value. */

	pthread_mutex_unlock(&gp_timer_prov->mutex);
	status = cl_timer_start(p_timer, time_ms);

	return (status);
}