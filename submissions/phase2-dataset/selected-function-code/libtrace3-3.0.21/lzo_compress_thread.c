static void *lzo_compress_thread(void *data)
{
	struct lzothread_t *me = (struct lzothread_t *)data;
	int err;
	char namebuf[17];

#ifdef PR_SET_NAME
	if (prctl(PR_GET_NAME, namebuf, 0,0,0) == 0) {
		char label[16];
		namebuf[16] = '\0'; /* Make sure it's NUL terminated */
		sprintf(label,"[lzo%d]",me->num);
		/* If the filename is too long, overwrite the last few bytes */
		if (strlen(namebuf)>=16-strlen(label)) {
			strcpy(namebuf+15-strlen(label),label);
		}
		else {
			strncat(namebuf," ",16);
			strncat(namebuf,label,16);
		}
		prctl(PR_SET_NAME, namebuf, 0,0,0);
	}
#endif

	pthread_mutex_lock(&me->mutex);
	while (!me->closing) {
		while (me->state != WAITING) {
			if (me->closing)
				break;
			pthread_cond_wait(&me->in_ready, &me->mutex);
		}
		if (me->closing)
			break;

		err=lzo_wwrite_block(
			me->inbuf.buffer, 
			me->inbuf.offset,
			&me->outbuf);

                if (err < 0)
                        break; 
		/* Make sure someone else hasn't clobbered us!*/
		assert(me->state == WAITING);
		me->state = FULL;
		pthread_cond_signal(&me->out_ready);
	}
	pthread_mutex_unlock(&me->mutex);

	return NULL;
}