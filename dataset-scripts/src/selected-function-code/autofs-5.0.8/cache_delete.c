int cache_delete(struct mapent_cache *mc, const char *key)
{
	struct mapent *me = NULL, *pred;
	u_int32_t hashval = hash(key, mc->size);
	int status, ret = CHE_OK;
	char this[PATH_MAX];

	strcpy(this, key);

	me = mc->hash[hashval];
	if (!me) {
		ret = CHE_FAIL;
		goto done;
	}

	while (me->next != NULL) {
		pred = me;
		me = me->next;
		if (strcmp(this, me->key) == 0) {
			if (me->multi && !list_empty(&me->multi_list)) {
				ret = CHE_FAIL;
				goto done;
			}
			pred->next = me->next;
			status = pthread_rwlock_destroy(&me->multi_rwlock);
			if (status)
				fatal(status);
			ino_index_lock(mc);
			list_del(&me->ino_index);
			ino_index_unlock(mc);
			free(me->key);
			if (me->mapent)
				free(me->mapent);
			free(me);
			me = pred;
		}
	}

	me = mc->hash[hashval];
	if (!me)
		goto done;

	if (strcmp(this, me->key) == 0) {
		if (me->multi && !list_empty(&me->multi_list)) {
			ret = CHE_FAIL;
			goto done;
		}
		mc->hash[hashval] = me->next;
		status = pthread_rwlock_destroy(&me->multi_rwlock);
		if (status)
			fatal(status);
		ino_index_lock(mc);
		list_del(&me->ino_index);
		ino_index_unlock(mc);
		free(me->key);
		if (me->mapent)
			free(me->mapent);
		free(me);
	}
done:
	return ret;
}