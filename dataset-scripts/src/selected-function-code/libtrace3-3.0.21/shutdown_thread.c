static void shutdown_thread(iow_t *iow, struct lzothread_t *thread)
{
	pthread_mutex_lock(&thread->mutex);

	/* If this buffer is empty it shouldn't have any data in it, we should have taken
         * care of that before.
	 */
	/* thread->state == EMPTY implies thread->inbuf.offset == 0 */
	assert(!(thread->state == EMPTY) || thread->inbuf.offset == 0);

	while (thread->state == WAITING) {
		pthread_cond_wait(
			&thread->out_ready,
			&thread->mutex);
	}
	if (thread->state == FULL) {
		wandio_wwrite(DATA(iow)->child,
				thread->outbuf.buffer,
				thread->outbuf.offset);
		thread->state = EMPTY;
		thread->inbuf.offset = 0;
	}
	/* Now the thread should be empty, so ask it to shut down */
	assert(thread->state == EMPTY && thread->inbuf.offset == 0);
	thread->closing = true;
	pthread_cond_signal(&thread->in_ready);
	pthread_mutex_unlock(&thread->mutex);
	/* And wait for it to die */
	pthread_join(thread->thread,NULL);
}