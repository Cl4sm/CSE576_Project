static void check_update_map_sources(struct master_mapent *entry, int readall)
{
	struct map_source *source, *last;
	struct autofs_point *ap;
	int map_stale = 0;

	if (readall)
		map_stale = 1;

	ap = entry->ap;

	master_source_writelock(entry);

	last = NULL;
	source = entry->maps;
	while (source) {
		if (readall)
			source->stale = 1;

		/*
		 * If a map source is no longer valid and all it's
		 * entries have expired away we can get rid of it.
		 */
		if (entry->age > source->age) {
			struct mapent *me;
			cache_readlock(source->mc);
			me = cache_lookup_first(source->mc);
			if (!me) {
				struct map_source *next = source->next;

				cache_unlock(source->mc);

				if (!last)
					entry->maps = next;
				else
					last->next = next;

				if (entry->maps == source)
					entry->maps = next;

				master_free_map_source(source, 1);

				source = next;
				continue;
			} else {
				source->stale = 1;
				map_stale = 1;
			}
			cache_unlock(source->mc);
		}
		last = source;
		source = source->next;
	}

	master_source_unlock(entry);

	/* The map sources have changed */
	if (map_stale)
		st_add_task(ap, ST_READMAP);

	return;
}