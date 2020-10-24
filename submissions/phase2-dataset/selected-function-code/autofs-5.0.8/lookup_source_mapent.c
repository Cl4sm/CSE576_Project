struct mapent *lookup_source_mapent(struct autofs_point *ap, const char *key, unsigned int type)
{
	struct master_mapent *entry = ap->entry;
	struct map_source *map;
	struct mapent_cache *mc;
	struct mapent *me = NULL;

	map = entry->maps;
	while (map) {
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

	if (me && me->mc != mc)
		error(LOGOPT_ANY, "mismatching mc in cache", me->key);

	return me;
}