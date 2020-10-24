int cache_delete_offset(struct mapent_cache *mc, const char *key)
{
	u_int32_t hashval = hash(key, mc->size);
	struct mapent *me = NULL, *pred;
	int status;

	me = mc->hash[hashval];
	if (!me)
		return CHE_FAIL;

	if (strcmp(key, me->key) == 0) {
		if (me->multi && me->multi == me)
			return CHE_FAIL;
		mc->hash[hashval] = me->next;
		goto delete;
	}

	while (me->next != NULL) {
		pred = me;
		me = me->next;
		if (strcmp(key, me->key) == 0) {
			if (me->multi && me->multi == me)
				return CHE_FAIL;
			pred->next = me->next;
			goto delete;
		}
	}

	return CHE_FAIL;

delete:
	status = pthread_rwlock_destroy(&me->multi_rwlock);
	if (status)
		fatal(status);
	list_del(&me->multi_list);
	ino_index_lock(mc);
	list_del(&me->ino_index);
	ino_index_unlock(mc);
	free(me->key);
	if (me->mapent)
		free(me->mapent);
	free(me);

	return CHE_OK;
}