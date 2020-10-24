static int update_hosts_mounts(struct autofs_point *ap,
			       struct map_source *source, time_t age,
			       struct lookup_context *ctxt)
{
	struct mapent_cache *mc;
	struct mapent *me;
	char *mapent;
	int ret;

	mc = source->mc;

	pthread_cleanup_push(cache_lock_cleanup, mc);
	cache_writelock(mc);
	me = cache_lookup_first(mc);
	while (me) {
		/* Hosts map entry not yet expanded or already expired */
		if (!me->multi)
			goto next;

		debug(ap->logopt, MODPREFIX "get list of exports for %s", me->key);

		mapent = get_exports(ap, me->key);
		if (mapent) {
			cache_update(mc, source, me->key, mapent, age);
			free(mapent);
		}
next:
		me = cache_lookup_next(mc, me);
	}
	pthread_cleanup_pop(1);

	pthread_cleanup_push(cache_lock_cleanup, mc);
	cache_readlock(mc);
	me = cache_lookup_first(mc);
	while (me) {
		/*
		 * Hosts map entry not yet expanded, already expired
		 * or not the base of the tree
		 */
		if (!me->multi || me->multi != me)
			goto cont;

		debug(ap->logopt, MODPREFIX
		      "attempt to update exports for exports for %s", me->key);

		master_source_current_wait(ap->entry);
		ap->entry->current = source;
		ap->flags |= MOUNT_FLAG_REMOUNT;
		ret = ctxt->parse->parse_mount(ap, me->key, strlen(me->key),
					       me->mapent, ctxt->parse->context);
		ap->flags &= ~MOUNT_FLAG_REMOUNT;
cont:
		me = cache_lookup_next(mc, me);
	}
	pthread_cleanup_pop(1);

	return NSS_STATUS_SUCCESS;
}