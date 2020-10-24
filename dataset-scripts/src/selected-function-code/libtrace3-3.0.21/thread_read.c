static off_t thread_read(io_t *state, void *buffer, off_t len)
{
	int slice;
	int copied=0;
	int newbuffer;

	while(len>0) {
		pthread_mutex_lock(&DATA(state)->mutex);
		
		/* Wait for the reader thread to provide us with some data */
		while (INBUFFER(state).state == EMPTY) {
			++read_waits;
			pthread_cond_wait(&DATA(state)->data_ready, &DATA(state)->mutex);

		}
		
		/* Check for errors and EOF */
		if (INBUFFER(state).len <1) {

			if (copied<1) {
				errno=EIO; /* FIXME: Preserve the errno from the other thread */
				copied = INBUFFER(state).len;
			}

			pthread_mutex_unlock(&DATA(state)->mutex);
			return copied;
		}

		/* Copy the next available slice into the main buffer */
		slice=min( INBUFFER(state).len-DATA(state)->offset,len);

		pthread_mutex_unlock(&DATA(state)->mutex);
				
		memcpy(
			buffer,
			INBUFFER(state).buffer+DATA(state)->offset,
			slice
			);

		buffer+=slice;
		len-=slice;
		copied+=slice;

		pthread_mutex_lock(&DATA(state)->mutex);
		DATA(state)->offset+=slice;
		newbuffer = DATA(state)->in_buffer;
		
		/* If we've read everything from the current slice, let the
		 * read thread know that there is now more space available 
		 * and start reading from the next slice */
		if (DATA(state)->offset >= INBUFFER(state).len) {
			INBUFFER(state).state = EMPTY;
			pthread_cond_signal(&DATA(state)->space_avail);
			newbuffer = (newbuffer+1) % max_buffers;
			DATA(state)->offset = 0;
		}

		pthread_mutex_unlock(&DATA(state)->mutex);

		DATA(state)->in_buffer = newbuffer;
	}
	return copied;
}