static void update_map_cache(struct autofs_point *ap, const char *path)
{
	struct map_source *map;
	struct mapent_cache *mc;
	const char *key;

	if (ap->type == LKP_INDIRECT)
		key = strrchr(path, '/') + 1;
	else
		key = path;

	map = ap->entry->maps;
	while (map) {
		struct mapent *me = NULL;

		/* Skip current, in-use cache */
		if (ap->entry->age <= map->age) {
			map = map->next;
			continue;
		}

		mc = map->mc;
		/* If the lock is busy try later */
		if (cache_try_writelock(mc)) {
			me = cache_lookup_distinct(mc, key);
			if (me && me->ioctlfd == -1)
				cache_delete(mc, key);
			cache_unlock(mc);
		}

		map = map->next;
	}

	return;
}