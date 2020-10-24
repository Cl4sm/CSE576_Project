static void *thread_consumer(void *userdata)
{
	int buffer=0;
	bool running = true;
	iow_t *state = (iow_t *) userdata;

#ifdef PR_SET_NAME
	char namebuf[17];
	if (prctl(PR_GET_NAME, namebuf, 0,0,0) == 0) {
		namebuf[16] = '\0'; /* Make sure it's NUL terminated */
		/* If the filename is too long, overwrite the last few bytes */
		if (strlen(namebuf)>9) {
			strcpy(namebuf+10,"[iow]");
		}
		else {
			strncat(namebuf," [iow]",16);
		}
		prctl(PR_SET_NAME, namebuf, 0,0,0);
	}
#endif

	pthread_mutex_lock(&DATA(state)->mutex);
	do {
		/* Wait for data that we can write */
		while (DATA(state)->buffer[buffer].state == EMPTY) {
			/* Unless, of course, the program is over! */
			if (DATA(state)->closing)
				break;
			pthread_cond_wait(&DATA(state)->data_ready,
					&DATA(state)->mutex);
		}
		
		/* Empty the buffer using the child writer */
		pthread_mutex_unlock(&DATA(state)->mutex);
		wandio_wwrite(
				DATA(state)->iow,
				DATA(state)->buffer[buffer].buffer,
				DATA(state)->buffer[buffer].len);
		pthread_mutex_lock(&DATA(state)->mutex);

		/* If we've not reached the end of the file keep going */
		running = ( DATA(state)->buffer[buffer].len > 0 );
		DATA(state)->buffer[buffer].len = 0;
		DATA(state)->buffer[buffer].state = EMPTY;

		/* Signal that we've freed up another buffer for the main
		 * thread to copy data into */
		pthread_cond_signal(&DATA(state)->space_avail);


		/* Move on to the next buffer */
		buffer=(buffer+1) % BUFFERS;

	} while(running);

	/* If we reach here, it's all over so start tidying up */
	wandio_wdestroy(DATA(state)->iow);

	pthread_mutex_unlock(&DATA(state)->mutex);
	return NULL;
}