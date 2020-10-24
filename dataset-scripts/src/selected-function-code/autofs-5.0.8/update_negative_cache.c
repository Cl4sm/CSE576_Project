static void update_negative_cache(struct autofs_point *ap, struct map_source *source, const char *name)
{
	struct master_mapent *entry = ap->entry;
	struct map_source *map;
	struct mapent *me;

	/* Don't update negative cache for included maps */ 
	if (source && source->depth)
		return;

	/* Have we recorded the lookup fail for negative caching? */
	me = lookup_source_mapent(ap, name, LKP_DISTINCT);
	if (me)
		/*
		 *  Already exists in the cache, the mount fail updates
		 *  will update negative timeout status.
		 */
		cache_unlock(me->mc);
	else {
		/* Notify only once after fail */
		logmsg("key \"%s\" not found in map source(s).", name);

		/* Doesn't exist in any source, just add it somewhere */
		if (source)
			map = source;
		else
			map = entry->maps;
		if (map) {
			time_t now = time(NULL);
			int rv = CHE_FAIL;

			cache_writelock(map->mc);
			rv = cache_update(map->mc, map, name, NULL, now);
			if (rv != CHE_FAIL) {
				me = cache_lookup_distinct(map->mc, name);
				me->status = now + ap->negative_timeout;
			}
			cache_unlock(map->mc);
		}
	}
	return;
}