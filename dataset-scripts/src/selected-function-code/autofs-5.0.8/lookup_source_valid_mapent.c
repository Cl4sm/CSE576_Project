struct mapent *lookup_source_valid_mapent(struct autofs_point *ap, const char *key, unsigned int type)
{
	struct master_mapent *entry = ap->entry;
	struct map_source *map;
	struct mapent_cache *mc;
	struct mapent *me = NULL;

	map = entry->maps;
	while (map) {
		/*
		 * Only consider map sources that have been read since
		 * the map entry was last updated.
		 */
		if (ap->entry->age > map->age) {
			map = map->next;
			continue;
		}

		mc = map->mc;
		cache_readlock(mc);
		if (type == LKP_DISTINCT)
			me = cache_lookup_distinct(mc, key);
		else
			me = cache_lookup(mc, key);
		if (me)
			break;
		cache_unlock(mc);
		map = map->next;
	}

	return me;
}