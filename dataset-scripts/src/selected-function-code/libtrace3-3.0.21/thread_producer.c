static void *thread_producer(void* userdata)
{
	io_t *state = (io_t*) userdata;
	int buffer=0;
	bool running = true;

#ifdef PR_SET_NAME
	char namebuf[17];
	if (prctl(PR_GET_NAME, namebuf, 0,0,0) == 0) {
		namebuf[16] = '\0'; /* Make sure it's NUL terminated */
		/* If the filename is too long, overwrite the last few bytes */
		if (strlen(namebuf)>9) {
			strcpy(namebuf+10,"[ior]");
		}
		else {
			strncat(namebuf," [ior]",16);
		}
		prctl(PR_SET_NAME, namebuf, 0,0,0);
	}
#endif

	pthread_mutex_lock(&DATA(state)->mutex);
	do {
		/* If all the buffers are full, we need to wait for one to
		 * become free otherwise we have nowhere to write to! */
		while (DATA(state)->buffer[buffer].state == FULL) {
			if (DATA(state)->closing)
				break;
			pthread_cond_wait(&DATA(state)->space_avail, &DATA(state)->mutex);
		}

		/* Don't bother reading any more data if we are shutting up
		 * shop */
		if (DATA(state)->closing) {
			break;
		}
		pthread_mutex_unlock(&DATA(state)->mutex);

		/* Get the parent reader to fill the buffer */
		DATA(state)->buffer[buffer].len=wandio_read(
				DATA(state)->io,
				DATA(state)->buffer[buffer].buffer,
				sizeof(DATA(state)->buffer[buffer].buffer));

		pthread_mutex_lock(&DATA(state)->mutex);

		DATA(state)->buffer[buffer].state = FULL;

		/* If we've not reached the end of the file keep going */
		running = (DATA(state)->buffer[buffer].len > 0 );

		/* Signal that there is data available for the main thread */
		pthread_cond_signal(&DATA(state)->data_ready);

		/* Move on to the next buffer */
		buffer=(buffer+1) % max_buffers;

	} while(running);

	/* If we reach here, it's all over so start tidying up */
	wandio_destroy(DATA(state)->io);

	pthread_cond_signal(&DATA(state)->data_ready);
	pthread_mutex_unlock(&DATA(state)->mutex);

	return NULL;
}