int cache_add(struct mapent_cache *mc, struct map_source *ms, const char *key, const char *mapent, time_t age)
{
	struct mapent *me, *existing = NULL;
	char *pkey, *pent;
	u_int32_t hashval = hash(key, mc->size);
	int status;

	me = (struct mapent *) malloc(sizeof(struct mapent));
	if (!me)
		return CHE_FAIL;

	pkey = malloc(strlen(key) + 1);
	if (!pkey) {
		free(me);
		return CHE_FAIL;
	}
	me->key = strcpy(pkey, key);

	if (mapent) {
		pent = malloc(strlen(mapent) + 1);
		if (!pent) {
			free(me);
			free(pkey);
			return CHE_FAIL;
		}
		me->mapent = strcpy(pent, mapent);
	} else
		me->mapent = NULL;

	me->age = age;
	me->status = 0;
	me->mc = mc;
	me->source = ms;
	INIT_LIST_HEAD(&me->ino_index);
	INIT_LIST_HEAD(&me->multi_list);
	me->multi = NULL;
	me->parent = NULL;
	me->ioctlfd = -1;
	me->dev = (dev_t) -1;
	me->ino = (ino_t) -1;
	me->flags = 0;

	status = pthread_rwlock_init(&me->multi_rwlock, NULL);
	if (status)
		fatal(status);

	/* 
	 * We need to add to the end if values exist in order to
	 * preserve the order in which the map was read on lookup.
	 */
	existing = cache_lookup_distinct(mc, key);
	if (!existing) {
		me->next = mc->hash[hashval];
		mc->hash[hashval] = me;
	} else {
		while (1) {
			struct mapent *next;
		
			next = cache_lookup_key_next(existing);
			if (!next)
				break;

			existing = next;
		}
		me->next = existing->next;
		existing->next = me;
	}
	return CHE_OK;
}