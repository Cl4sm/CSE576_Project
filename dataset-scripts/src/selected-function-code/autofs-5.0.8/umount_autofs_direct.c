int umount_autofs_direct(struct autofs_point *ap)
{
	struct map_source *map;
	struct mapent_cache *nc, *mc;
	struct mnt_list *mnts;
	struct mapent *me, *ne;

	mnts = tree_make_mnt_tree(_PROC_MOUNTS, "/");
	pthread_cleanup_push(mnts_cleanup, mnts);
	nc = ap->entry->master->nc;
	cache_readlock(nc);
	pthread_cleanup_push(cache_lock_cleanup, nc);
	map = ap->entry->maps;
	while (map) {
		mc = map->mc;
		pthread_cleanup_push(cache_lock_cleanup, mc);
		cache_readlock(mc);
		me = cache_enumerate(mc, NULL);
		while (me) {
			ne = cache_lookup_distinct(nc, me->key);
			if (ne && map->master_line > ne->age) {
				me = cache_enumerate(mc, me);
				continue;
			}

			/* TODO: check return, locking me */
			do_umount_autofs_direct(ap, mnts, me);

			me = cache_enumerate(mc, me);
		}
		pthread_cleanup_pop(1);
		map = map->next;
	}
	pthread_cleanup_pop(1);
	pthread_cleanup_pop(1);

	close(ap->state_pipe[0]);
	close(ap->state_pipe[1]);
	if (ap->pipefd >= 0)
		close(ap->pipefd);
	if (ap->kpipefd >= 0) {
		close(ap->kpipefd);
		ap->kpipefd = -1;
	}

	return 0;
}