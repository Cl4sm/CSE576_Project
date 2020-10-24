io_t *thread_open(io_t *parent)
{
	io_t *state;

	if (!parent) {
		return NULL;
	}
	

	state = malloc(sizeof(io_t));
	state->data = calloc(1,sizeof(struct state_t));
	state->source = &thread_source;

	DATA(state)->buffer = (struct buffer_t *)malloc(sizeof(struct buffer_t) * max_buffers);
	memset(DATA(state)->buffer, 0, sizeof(struct buffer_t) * max_buffers);
	DATA(state)->in_buffer = 0;
	DATA(state)->offset = 0;
	pthread_mutex_init(&DATA(state)->mutex,NULL);
	pthread_cond_init(&DATA(state)->data_ready,NULL);
	pthread_cond_init(&DATA(state)->space_avail,NULL);

	DATA(state)->io = parent;
	DATA(state)->closing = false;

	/* Create the reading thread */
	pthread_create(&DATA(state)->producer,NULL,thread_producer,state);

	return state;
}