int cache_update(struct mapent_cache *mc, struct map_source *ms, const char *key, const char *mapent, time_t age)
{
	unsigned logopt = mc->ap ? mc->ap->logopt : master_get_logopt();
	struct mapent *me = NULL;
	char *pent;
	int ret = CHE_OK;

	me = cache_lookup(mc, key);
	while (me && me->source != ms)
		me = cache_lookup_key_next(me);
	if (!me || (!strcmp(me->key, "*") && strcmp(key, "*"))) {
		ret = cache_add(mc, ms, key, mapent, age);
		if (!ret) {
			debug(logopt, "failed for %s", key);
			return CHE_FAIL;
		}
		ret = CHE_UPDATED;
	} else {
		/* Already seen one of these */
		if (me->age == age)
			return CHE_OK;

		if (!mapent) {
			if (me->mapent)
				free(me->mapent);
			me->mapent = NULL;
		} else if (!me->mapent || strcmp(me->mapent, mapent) != 0) {
			pent = malloc(strlen(mapent) + 1);
			if (pent == NULL)
				return CHE_FAIL;
			if (me->mapent)
				free(me->mapent);
			me->mapent = strcpy(pent, mapent);
			ret = CHE_UPDATED;
		}
		me->age = age;
	}
	return ret;
}