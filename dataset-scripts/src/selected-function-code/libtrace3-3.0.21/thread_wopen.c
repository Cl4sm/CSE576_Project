iow_t *thread_wopen(iow_t *child)
{
	iow_t *state;

	if (!child) {
		return NULL;
	}
	

	state = malloc(sizeof(iow_t));
	state->data = calloc(1,sizeof(struct state_t));
	state->source = &thread_wsource;

	DATA(state)->out_buffer = 0;
	DATA(state)->offset = 0;
	pthread_mutex_init(&DATA(state)->mutex,NULL);
	pthread_cond_init(&DATA(state)->data_ready,NULL);
	pthread_cond_init(&DATA(state)->space_avail,NULL);

	DATA(state)->iow = child;
	DATA(state)->closing = false;

	/* Start the writer thread */
	pthread_create(&DATA(state)->consumer,NULL,thread_consumer,state);

	return state;
}