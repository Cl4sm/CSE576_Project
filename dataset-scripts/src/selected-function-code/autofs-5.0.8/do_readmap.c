static void *do_readmap(void *arg)
{
	struct autofs_point *ap;
	struct map_source *map;
	struct mapent_cache *nc, *mc;
	struct readmap_args *ra;
	struct mnt_list *mnts;
	int status;
	time_t now;

	ra = (struct readmap_args *) arg;

	status = pthread_mutex_lock(&ra->mutex);
	if (status)
		fatal(status);

	ap = ra->ap;
	now = ra->now;

	ra->signaled = 1;
	status = pthread_cond_signal(&ra->cond);
	if (status) {
		error(ap->logopt, "failed to signal expire condition");
		pthread_mutex_unlock(&ra->mutex);
		fatal(status);
	}

	status = pthread_mutex_unlock(&ra->mutex);
	if (status)
		fatal(status);

	pthread_cleanup_push(do_readmap_cleanup, ra);

	info(ap->logopt, "re-reading map for %s", ap->path);

	pthread_cleanup_push(master_mutex_lock_cleanup, NULL);
	master_mutex_lock();
	status = lookup_nss_read_map(ap, NULL, now);
	if (!status)
		pthread_exit(NULL);
	pthread_cleanup_pop(1);

	if (ap->type == LKP_INDIRECT) {
		struct ioctl_ops *ops = get_ioctl_ops();
		time_t timeout = ap->entry->maps->exp_timeout;
		ap->exp_runfreq = (timeout + CHECK_RATIO - 1) / CHECK_RATIO;
		ops->timeout(ap->logopt, ap->ioctlfd, timeout);
		lookup_prune_cache(ap, now);
		status = lookup_ghost(ap, ap->path);
	} else {
		struct mapent *me;

		mnts = tree_make_mnt_tree(_PROC_MOUNTS, "/");
		pthread_cleanup_push(tree_mnts_cleanup, mnts);
		nc = ap->entry->master->nc;
		cache_readlock(nc);
		pthread_cleanup_push(cache_lock_cleanup, nc);
		master_source_readlock(ap->entry);
		pthread_cleanup_push(master_source_lock_cleanup, ap->entry);
		map = ap->entry->maps;
		while (map) {
			/* Is map source up to date or no longer valid */
			if (!map->stale) {
				map = map->next;
				continue;
			}
			mc = map->mc;
			pthread_cleanup_push(cache_lock_cleanup, mc);
			cache_readlock(mc);
			me = cache_enumerate(mc, NULL);
			while (me) {
				do_readmap_mount(ap, mnts, map, me, now);
				me = cache_enumerate(mc, me);
			}
			lookup_prune_one_cache(ap, map->mc, now);
			pthread_cleanup_pop(1);
			map->stale = 0;
			map = map->next;
		}
		pthread_cleanup_pop(1);
		pthread_cleanup_pop(1);
		pthread_cleanup_pop(1);
	}

	pthread_cleanup_pop(1);

	return NULL;
}