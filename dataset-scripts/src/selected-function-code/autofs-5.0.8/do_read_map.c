static int do_read_map(struct autofs_point *ap, struct map_source *map, time_t age)
{
	struct lookup_mod *lookup;
	int status;

	lookup = open_lookup(map->type, "", map->format, map->argc, map->argv);
	if (!lookup) {
		debug(ap->logopt, "lookup module %s failed", map->type);
		return NSS_STATUS_UNAVAIL;
	}

	master_source_writelock(ap->entry);
	if (map->lookup)
		close_lookup(map->lookup);
	map->lookup = lookup;
	master_source_unlock(ap->entry);

	if (!map->stale)
		return NSS_STATUS_SUCCESS;

	master_source_current_wait(ap->entry);
	ap->entry->current = map;

	status = lookup->lookup_read_map(ap, age, lookup->context);

	if (status != NSS_STATUS_SUCCESS)
		map->stale = 0;

	/*
	 * For maps that don't support enumeration return success
	 * and do whatever we must to have autofs function with an
	 * empty map entry cache.
	 *
	 * For indirect maps that use the browse option, when the
	 * server is unavailable continue as best we can with
	 * whatever we have in the cache, if anything.
	 */
	if (status == NSS_STATUS_UNKNOWN ||
	   (ap->type == LKP_INDIRECT && status == NSS_STATUS_UNAVAIL))
		return NSS_STATUS_SUCCESS;

	return status;
}