void *tur_thread(void *ctx)
{
	struct tur_checker_context *ct = ctx;
	int state;

	condlog(3, "%d:%d: tur checker starting up", TUR_DEVT(ct));

	ct->message[0] = '\0';
	/* This thread can be canceled, so setup clean up */
	tur_thread_cleanup_push(ct)

	/* TUR checker start up */
	pthread_mutex_lock(&ct->lock);
	ct->state = PATH_PENDING;
	pthread_mutex_unlock(&ct->lock);

	state = tur_check(ct->fd, ct->timeout, ct->message);

	/* TUR checker done */
	pthread_mutex_lock(&ct->lock);
	ct->state = state;
	pthread_mutex_unlock(&ct->lock);
	pthread_cond_signal(&ct->active);

	condlog(3, "%d:%d: tur checker finished, state %s",
		TUR_DEVT(ct), checker_state_name(state));
	tur_thread_cleanup_pop(ct);
	return ((void *)0);
}
