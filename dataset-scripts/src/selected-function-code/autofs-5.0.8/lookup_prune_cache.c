int lookup_prune_cache(struct autofs_point *ap, time_t age)
{
	struct master_mapent *entry = ap->entry;
	struct map_source *map;

	pthread_cleanup_push(master_source_lock_cleanup, entry);
	master_source_readlock(entry);

	map = entry->maps;
	while (map) {
		/* Is the map stale */
		if (!map->stale) {
			map = map->next;
			continue;
		}
		pthread_cleanup_push(cache_lock_cleanup, map->mc);
		cache_readlock(map->mc);
		lookup_prune_one_cache(ap, map->mc, age);
		pthread_cleanup_pop(1);
		clear_stale_instances(map);
		map->stale = 0;
		map = map->next;
	}

	pthread_cleanup_pop(1);

	return 1;
}