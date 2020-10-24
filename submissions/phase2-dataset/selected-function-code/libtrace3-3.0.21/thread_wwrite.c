static off_t thread_wwrite(iow_t *state, const char *buffer, off_t len)
{
	int slice;
	int copied=0;
	int newbuffer;

	pthread_mutex_lock(&DATA(state)->mutex);
	while(len>0) {

		/* Wait for there to be space available for us to write into */
		while (OUTBUFFER(state).state == FULL) {
			write_waits++;
			pthread_cond_wait(&DATA(state)->space_avail,
					&DATA(state)->mutex);
		}

		/* Copy out of our main buffer into the next available slice */
		slice=min( 
			(off_t)sizeof(OUTBUFFER(state).buffer)-DATA(state)->offset,
			len);
				
		pthread_mutex_unlock(&DATA(state)->mutex);
		memcpy(
			OUTBUFFER(state).buffer+DATA(state)->offset,
			buffer,
			slice
			);
		pthread_mutex_lock(&DATA(state)->mutex);

		DATA(state)->offset += slice;
		OUTBUFFER(state).len += slice;

		buffer += slice;
		len -= slice;
		copied += slice;
		newbuffer = DATA(state)->out_buffer;

		/* If we've filled a buffer, move on to the next one and 
		 * signal to the write thread that there is something for it
		 * to do */
		if (DATA(state)->offset >= (off_t)sizeof(OUTBUFFER(state).buffer)) {
			OUTBUFFER(state).state = FULL;
			pthread_cond_signal(&DATA(state)->data_ready);
			DATA(state)->offset = 0;
			newbuffer = (newbuffer+1) % BUFFERS;
		}

		DATA(state)->out_buffer = newbuffer;
	}

	pthread_mutex_unlock(&DATA(state)->mutex);
	return copied;
}