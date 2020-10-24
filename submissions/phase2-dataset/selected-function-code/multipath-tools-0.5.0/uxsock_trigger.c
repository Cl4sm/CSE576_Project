uxsock_trigger (char * str, char ** reply, int * len, void * trigger_data)
{
	struct vectors * vecs;
	int r;

	*reply = NULL;
	*len = 0;
	vecs = (struct vectors *)trigger_data;

	pthread_cleanup_push(cleanup_lock, &vecs->lock);
	lock(vecs->lock);
	pthread_testcancel();

	r = parse_cmd(str, reply, len, vecs);

	if (r > 0) {
		*reply = STRDUP("fail\n");
		*len = strlen(*reply) + 1;
		r = 1;
	}
	else if (!r && *len == 0) {
		*reply = STRDUP("ok\n");
		*len = strlen(*reply) + 1;
		r = 0;
	}
	/* else if (r < 0) leave *reply alone */

	lock_cleanup_pop(vecs->lock);
	return r;
}
