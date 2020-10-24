void cl_timer_stop(IN cl_timer_t * const p_timer)
{
	CL_ASSERT(p_timer);
	CL_ASSERT(p_timer->state == CL_INITIALIZED);

	pthread_mutex_lock(&gp_timer_prov->mutex);
	switch (p_timer->timer_state) {
	case CL_TIMER_RUNNING:
		/* Wait for the callback to complete. */
		pthread_cond_wait(&p_timer->cond, &gp_timer_prov->mutex);
		/* Timer could have been queued while we were waiting. */
		if (p_timer->timer_state != CL_TIMER_QUEUED)
			break;

	case CL_TIMER_QUEUED:
		/* Change the state of the timer. */
		p_timer->timer_state = CL_TIMER_IDLE;
		/* Remove the timer from the queue. */
		cl_qlist_remove_item(&gp_timer_prov->queue,
				     &p_timer->list_item);
		/*
		 * Signal the timer provider thread to move onto the
		 * next timer in the queue.
		 */
		pthread_cond_signal(&gp_timer_prov->cond);
		break;

	case CL_TIMER_IDLE:
		break;
	}
	pthread_mutex_unlock(&gp_timer_prov->mutex);
}