struct mapent *cache_lookup(struct mapent_cache *mc, const char *key)
{
	struct mapent *me = NULL;

	if (!key)
		return NULL;

	for (me = mc->hash[hash(key, mc->size)]; me != NULL; me = me->next) {
		if (strcmp(key, me->key) == 0)
			goto done;
	}

	me = cache_lookup_first(mc);
	if (me != NULL) {
		/* Can't have wildcard in direct map */
		if (*me->key == '/') {
			me = NULL;
			goto done;
		}

		for (me = mc->hash[hash("*", mc->size)]; me != NULL; me = me->next)
			if (strcmp("*", me->key) == 0)
				goto done;
	}
done:
	return me;
}