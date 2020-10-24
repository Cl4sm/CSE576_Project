cl_status_t cl_timer_start(IN cl_timer_t * const p_timer,
			   IN const uint32_t time_ms)
{
	cl_list_item_t *p_list_item;

	CL_ASSERT(p_timer);
	CL_ASSERT(p_timer->state == CL_INITIALIZED);

	pthread_mutex_lock(&gp_timer_prov->mutex);
	/* Signal the timer provider thread to wake up. */
	pthread_cond_signal(&gp_timer_prov->cond);

	/* Remove the timer from the queue if currently queued. */
	if (p_timer->timer_state == CL_TIMER_QUEUED)
		cl_qlist_remove_item(&gp_timer_prov->queue,
				     &p_timer->list_item);

	__cl_timer_calculate(time_ms, &p_timer->timeout);

	/* Add the timer to the queue. */
	if (cl_is_qlist_empty(&gp_timer_prov->queue)) {
		/* The timer list is empty.  Add to the head. */
		cl_qlist_insert_head(&gp_timer_prov->queue,
				     &p_timer->list_item);
	} else {
		/* Find the correct insertion place in the list for the timer. */
		p_list_item = cl_qlist_find_from_tail(&gp_timer_prov->queue,
						      __cl_timer_find, p_timer);

		/* Insert the timer. */
		cl_qlist_insert_next(&gp_timer_prov->queue, p_list_item,
				     &p_timer->list_item);
	}
	/* Set the state. */
	p_timer->timer_state = CL_TIMER_QUEUED;
	pthread_mutex_unlock(&gp_timer_prov->mutex);

	return (CL_SUCCESS);
}