static void lzo_wclose(iow_t *iow)
{
	const uint32_t zero = 0;
	int i;

	/* Flush the last buffer */
	pthread_mutex_lock(&get_next_thread(iow)->mutex);
	if (get_next_thread(iow)->state == EMPTY && get_next_thread(iow)->inbuf.offset != 0) {
		get_next_thread(iow)->state = WAITING;
		pthread_cond_signal(&get_next_thread(iow)->in_ready);
	}
	pthread_mutex_unlock(&get_next_thread(iow)->mutex);

	DATA(iow)->next_thread = 
			(DATA(iow)->next_thread+1) % DATA(iow)->threads;

	/* Right, now we have to shutdown all our threads -- in order */
	for(i=DATA(iow)->next_thread; i<DATA(iow)->threads; ++i) {
		shutdown_thread(iow,&DATA(iow)->thread[i]);
	}
	for(i=0; i<DATA(iow)->next_thread; ++i) {
		shutdown_thread(iow,&DATA(iow)->thread[i]);
	}

	/* Write out an end of file marker */
	wandio_wwrite(DATA(iow)->child,
		&zero,
		sizeof(zero));

	/* And clean everything up */
	wandio_wdestroy(DATA(iow)->child);
	free(DATA(iow)->thread);
	free(iow->data);
	free(iow);
}