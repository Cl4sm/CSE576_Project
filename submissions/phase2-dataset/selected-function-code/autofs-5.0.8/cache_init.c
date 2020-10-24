struct mapent_cache *cache_init(struct autofs_point *ap, struct map_source *map)
{
	struct mapent_cache *mc;
	unsigned int i;
	int status;

	if (map->mc)
		cache_release(map);

	mc = malloc(sizeof(struct mapent_cache));
	if (!mc)
		return NULL;

	mc->size = defaults_get_map_hash_table_size();

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

	cache_writelock(mc);

	for (i = 0; i < mc->size; i++) {
		mc->hash[i] = NULL;
		INIT_LIST_HEAD(&mc->ino_index[i]);
	}

	mc->ap = ap;
	mc->map = map;

	cache_unlock(mc);

	return mc;
}