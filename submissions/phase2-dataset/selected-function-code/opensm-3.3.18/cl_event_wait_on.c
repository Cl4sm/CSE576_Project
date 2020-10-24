cl_status_t cl_event_wait_on(IN cl_event_t * const p_event,
			     IN const uint32_t wait_us,
			     IN const boolean_t interruptible)
{
	cl_status_t status;
	int wait_ret;
	struct timespec timeout;
	struct timeval curtime;

	/* Make sure that the event was Started */
	CL_ASSERT(p_event->state == CL_INITIALIZED);

	pthread_mutex_lock(&p_event->mutex);

	/* Return immediately if the event is signalled. */
	if (p_event->signaled) {
		if (!p_event->manual_reset)
			p_event->signaled = FALSE;

		pthread_mutex_unlock(&p_event->mutex);
		return CL_SUCCESS;
	}

	/* If just testing the state, return CL_TIMEOUT. */
	if (wait_us == 0) {
		pthread_mutex_unlock(&p_event->mutex);
		return CL_TIMEOUT;
	}

	if (wait_us == EVENT_NO_TIMEOUT) {
		/* Wait for condition variable to be signaled or broadcast. */
		if (pthread_cond_wait(&p_event->condvar, &p_event->mutex))
			status = CL_NOT_DONE;
		else
			status = CL_SUCCESS;
	} else {
		/* Get the current time */
		if (gettimeofday(&curtime, NULL) == 0) {
			unsigned long n_sec =
			    (curtime.tv_usec + (wait_us % 1000000)) * 1000;
			timeout.tv_sec = curtime.tv_sec + (wait_us / 1000000)
			    + (n_sec / 1000000000);
			timeout.tv_nsec = n_sec % 1000000000;

			wait_ret = pthread_cond_timedwait(&p_event->condvar,
							  &p_event->mutex,
							  &timeout);
			if (wait_ret == 0)
				status =
				    (p_event->
				     signaled ? CL_SUCCESS : CL_NOT_DONE);
			else if (wait_ret == ETIMEDOUT)
				status = CL_TIMEOUT;
			else
				status = CL_NOT_DONE;
		} else
			status = CL_ERROR;
	}
	if (!p_event->manual_reset)
		p_event->signaled = FALSE;

	pthread_mutex_unlock(&p_event->mutex);
	return status;
}