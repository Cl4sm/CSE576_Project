int cache_update_offset(struct mapent_cache *mc, const char *mkey, const char *key, const char *mapent, time_t age)
{
	unsigned logopt = mc->ap ? mc->ap->logopt : master_get_logopt();
	struct mapent *me, *owner;
	int ret = CHE_OK;

	owner = cache_lookup_distinct(mc, mkey);
	if (!owner)
		return CHE_FAIL;

	me = cache_lookup_distinct(mc, key);
	if (me && me->age == age) {
		if (me == owner || strcmp(me->key, key) == 0)
			return CHE_DUPLICATE;
	}

	ret = cache_update(mc, owner->source, key, mapent, age);
	if (ret == CHE_FAIL) {
		warn(logopt, "failed to add key %s to cache", key);
		return CHE_FAIL;
	}

	me = cache_lookup_distinct(mc, key);
	if (me) {
		cache_add_ordered_offset(me, &owner->multi_list);
		me->multi = owner;
		goto done;
	}
	ret = CHE_FAIL;
done:
	return ret; 
}