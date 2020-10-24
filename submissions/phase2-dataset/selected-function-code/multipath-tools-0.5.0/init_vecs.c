init_vecs (void)
{
	struct vectors * vecs;

	vecs = (struct vectors *)MALLOC(sizeof(struct vectors));

	if (!vecs)
		return NULL;

	vecs->lock.mutex =
		(pthread_mutex_t *)MALLOC(sizeof(pthread_mutex_t));

	if (!vecs->lock.mutex)
		goto out;

	pthread_mutex_init(vecs->lock.mutex, NULL);
	vecs->lock.depth = 0;

	return vecs;

out:
	FREE(vecs);
	condlog(0, "failed to init paths");
	return NULL;
}
