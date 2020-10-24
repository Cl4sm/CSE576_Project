struct mapent_cache *cache_init_null_cache(struct master *master)
{
	struct mapent_cache *mc;
	unsigned int i;
	int status;

	mc = malloc(sizeof(struct mapent_cache));
	if (!mc)
		return NULL;

	mc->size = NULL_MAP_HASHSIZE;

	mc->hash = malloc(mc->size * sizeof(struct mapent *));
	if (!mc->hash) {
		free(mc);
		return NULL;
	}

	mc->ino_index = malloc(mc->size * sizeof(struct list_head));
	if (!mc->ino_index) {
		free(mc->hash);
		free(mc);
		return NULL;
	}

	status = pthread_mutex_init(&mc->ino_index_mutex, NULL);
	if (status)
		fatal(status);

	status = pthread_rwlock_init(&mc->rwlock, NULL);
	if (status)
		fatal(status);

	for (i = 0; i < mc->size; i++) {
		mc->hash[i] = NULL;
		INIT_LIST_HEAD(&mc->ino_index[i]);
	}

	mc->ap = NULL;
	mc->map = NULL;

	return mc;
}