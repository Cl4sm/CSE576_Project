static void *__cl_timer_prov_cb(IN void *const context)
{
	int ret;
	cl_timer_t *p_timer;

	pthread_mutex_lock(&gp_timer_prov->mutex);
	while (!gp_timer_prov->exit) {
		if (cl_is_qlist_empty(&gp_timer_prov->queue)) {
			/* Wait until we exit or a timer is queued. */
			/* cond wait does:
			 * pthread_cond_wait atomically unlocks the mutex (as per
			 * pthread_unlock_mutex) and waits for the condition variable
			 * cond to be signaled. The thread execution is suspended and
			 * does not consume any CPU time until the condition variable is
			 * signaled. The mutex must be locked by the calling thread on
			 * entrance to pthread_cond_wait. Before RETURNING TO THE
			 * CALLING THREAD, PTHREAD_COND_WAIT RE-ACQUIRES MUTEX (as per
			 * pthread_lock_mutex).
			 */
			ret = pthread_cond_wait(&gp_timer_prov->cond,
						&gp_timer_prov->mutex);
		} else {
			/*
			 * The timer elements are on the queue in expiration order.
			 * Get the first in the list to determine how long to wait.
			 */

			p_timer =
			    (cl_timer_t *) cl_qlist_head(&gp_timer_prov->queue);
			ret =
			    pthread_cond_timedwait(&gp_timer_prov->cond,
						   &gp_timer_prov->mutex,
						   &p_timer->timeout);

			/*
			   Sleep again on every event other than timeout and invalid
			   Note: EINVAL means that we got behind. This can occur when
			   we are very busy...
			 */
			if (ret != ETIMEDOUT && ret != EINVAL)
				continue;

			/*
			 * The timer expired.  Check the state in case it was cancelled
			 * after it expired but before we got a chance to invoke the
			 * callback.
			 */
			if (p_timer->timer_state != CL_TIMER_QUEUED)
				continue;

			/*
			 * Mark the timer as running to synchronize with its
			 * cancelation since we can't hold the mutex during the
			 * callback.
			 */
			p_timer->timer_state = CL_TIMER_RUNNING;

			/* Remove the item from the timer queue. */
			cl_qlist_remove_item(&gp_timer_prov->queue,
					     &p_timer->list_item);
			pthread_mutex_unlock(&gp_timer_prov->mutex);
			/* Invoke the callback. */
			p_timer->pfn_callback((void *)p_timer->context);

			/* Acquire the mutex again. */
			pthread_mutex_lock(&gp_timer_prov->mutex);
			/*
			 * Only set the state to idle if the timer has not been accessed
			 * from the callback
			 */
			if (p_timer->timer_state == CL_TIMER_RUNNING)
				p_timer->timer_state = CL_TIMER_IDLE;

			/*
			 * Signal any thread trying to manipulate the timer
			 * that expired.
			 */
			pthread_cond_signal(&p_timer->cond);
		}
	}
	gp_timer_prov->thread = 0;
	pthread_mutex_unlock(&gp_timer_prov->mutex);
	pthread_exit(NULL);
}