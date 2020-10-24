int mount_autofs_direct(struct autofs_point *ap)
{
	struct map_source *map;
	struct mapent_cache *nc, *mc;
	struct mapent *me, *ne, *nested;
	struct mnt_list *mnts;
	time_t now = time(NULL);

	if (strcmp(ap->path, "/-")) {
		error(ap->logopt, "expected direct map, exiting");
		return -1;
	}

	/* TODO: check map type */
	if (lookup_nss_read_map(ap, NULL, now))
		lookup_prune_cache(ap, now);
	else {
		error(ap->logopt, "failed to read direct map");
		return -1;
	}

	mnts = tree_make_mnt_tree(_PROC_MOUNTS, "/");
	pthread_cleanup_push(mnts_cleanup, mnts);
	pthread_cleanup_push(master_source_lock_cleanup, ap->entry);
	master_source_readlock(ap->entry);
	nc = ap->entry->master->nc;
	cache_readlock(nc);
	pthread_cleanup_push(cache_lock_cleanup, nc);
	map = ap->entry->maps;
	while (map) {
		time_t timeout;
		/*
		 * Only consider map sources that have been read since
		 * the map entry was last updated.
		 */
		if (ap->entry->age > map->age) {
			map = map->next;
			continue;
		}

		mc = map->mc;
		timeout = map->exp_timeout;
		cache_readlock(mc);
		pthread_cleanup_push(cache_lock_cleanup, mc);
		me = cache_enumerate(mc, NULL);
		while (me) {
			ne = cache_lookup_distinct(nc, me->key);
			if (ne) {
				if (map->master_line < ne->age) {
					/* TODO: check return, locking me */
					do_mount_autofs_direct(ap, mnts, me, timeout);
				}
				me = cache_enumerate(mc, me);
				continue;
			}

			nested = cache_partial_match(nc, me->key);
			if (nested) {
				error(ap->logopt,
				   "removing invalid nested null entry %s",
				   nested->key);
				nested = cache_partial_match(nc, me->key);
				if (nested)
					cache_delete(nc, nested->key);
			}

			/* TODO: check return, locking me */
			do_mount_autofs_direct(ap, mnts, me, timeout);

			me = cache_enumerate(mc, me);
		}
		pthread_cleanup_pop(1);
		map = map->next;
	}
	pthread_cleanup_pop(1);
	pthread_cleanup_pop(1);
	pthread_cleanup_pop(1);

	return 0;
}