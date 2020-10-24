void *expire_proc_indirect(void *arg)
{
	struct ioctl_ops *ops = get_ioctl_ops();
	struct autofs_point *ap;
	struct mapent *me = NULL;
	struct mnt_list *mnts = NULL, *next;
	struct expire_args *ea;
	struct expire_args ec;
	unsigned int now;
	int offsets, submnts, count;
	int ioctlfd, cur_state;
	int status, ret, left;

	ea = (struct expire_args *) arg;

	status = pthread_mutex_lock(&ea->mutex);
	if (status)
		fatal(status);

	ap = ec.ap = ea->ap;
	now = ea->when;
	ec.status = -1;

	ea->signaled = 1;
	status = pthread_cond_signal(&ea->cond);
	if (status)
		fatal(status);

	status = pthread_mutex_unlock(&ea->mutex);
	if (status)
		fatal(status);

	pthread_cleanup_push(expire_cleanup, &ec);

	left = 0;

	/* Get a list of real mounts and expire them if possible */
	mnts = get_mnt_list(_PROC_MOUNTS, ap->path, 0);
	pthread_cleanup_push(mnts_cleanup, mnts);
	for (next = mnts; next; next = next->next) {
		char *ind_key;
		int ret;

		if (!strcmp(next->fs_type, "autofs")) {
			/*
			 * If we have submounts check if this path lives below
			 * one of them and pass on the state change.
			 */
			pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &cur_state);
			if (strstr(next->opts, "indirect"))
				master_notify_submount(ap, next->path, ap->state);
			else if (strstr(next->opts, "offset")) {
				struct map_source *map;
				struct mapent_cache *mc = NULL;
				struct mapent *me = NULL;
				struct stat st;

				/* It's got a mount, deal with in the outer loop */
				if (is_mounted(_PATH_MOUNTED, next->path, MNTS_REAL)) {
					pthread_setcancelstate(cur_state, NULL);
					continue;
				}

				/* Don't touch submounts */
				if (master_find_submount(ap, next->path)) {
					pthread_setcancelstate(cur_state, NULL);
					continue;
				}

				master_source_writelock(ap->entry);

				map = ap->entry->maps;
				while (map) {
					mc = map->mc;
					cache_writelock(mc);
					me = cache_lookup_distinct(mc, next->path);
					if (me)
						break;
					cache_unlock(mc);
					map = map->next;
				}

				if (!mc || !me) {
					master_source_unlock(ap->entry);
					pthread_setcancelstate(cur_state, NULL);
					continue;
				}

				if (me->ioctlfd == -1) {
					cache_unlock(mc);
					master_source_unlock(ap->entry);
					pthread_setcancelstate(cur_state, NULL);
					continue;
				}

				/* Check for manual umount */
				if (fstat(me->ioctlfd, &st) == -1 ||
				    !count_mounts(ap->logopt, me->key, st.st_dev)) {
					ops->close(ap->logopt, me->ioctlfd);
					me->ioctlfd = -1;
				}

				cache_unlock(mc);
				master_source_unlock(ap->entry);
			}

			pthread_setcancelstate(cur_state, NULL);
			continue;
		}

		if (ap->state == ST_EXPIRE || ap->state == ST_PRUNE)
			pthread_testcancel();

		/*
		 * If the mount corresponds to an offset trigger then
		 * the key is the path, otherwise it's the last component.
		 */
		ind_key = strrchr(next->path, '/');
		if (ind_key)
			ind_key++;

		/*
		 * If me->key starts with a '/' and it's not an autofs
		 * filesystem it's a nested mount and we need to use
		 * the ioctlfd of the mount to send the expire.
		 * Otherwise it's a top level indirect mount (possibly
		 * with offsets in it) and we use the usual ioctlfd.
		 */
		pthread_cleanup_push(master_source_lock_cleanup, ap->entry);
		master_source_readlock(ap->entry);
		me = lookup_source_mapent(ap, next->path, LKP_DISTINCT);
		if (!me && ind_key)
			me = lookup_source_mapent(ap, ind_key, LKP_NORMAL);
		pthread_cleanup_pop(1);
		if (!me)
			continue;

		if (*me->key == '/') {
			ioctlfd = me->ioctlfd;
		} else {
			ioctlfd = ap->ioctlfd;
		}
		cache_unlock(me->mc);

		debug(ap->logopt, "expire %s", next->path);

		pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &cur_state);
		ret = ops->expire(ap->logopt, ioctlfd, next->path, now);
		if (ret)
			left++;
		pthread_setcancelstate(cur_state, NULL);
	}

	/*
	 * If there are no more real mounts left we could still
	 * have some offset mounts with no '/' offset so we need to
	 * umount them here.
	 */
	if (mnts) {
		int retries;
		pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &cur_state);
		retries = (count_mounts(ap->logopt, ap->path, ap->dev) + 1);
		while (retries--) {
			ret = ops->expire(ap->logopt, ap->ioctlfd, ap->path, now);
			if (ret)
				left++;
		}
		pthread_setcancelstate(cur_state, NULL);
	}
	pthread_cleanup_pop(1);

	count = offsets = submnts = 0;
	mnts = get_mnt_list(_PROC_MOUNTS, ap->path, 0);
	pthread_cleanup_push(mnts_cleanup, mnts);
	/* Are there any real mounts left */
	for (next = mnts; next; next = next->next) {
		if (strcmp(next->fs_type, "autofs"))
			count++;
		else {
			if (strstr(next->opts, "indirect"))
				submnts++;
			else
				offsets++;
		}
	}
	pthread_cleanup_pop(1);

	if (submnts)
		info(ap->logopt,
		     "%d submounts remaining in %s", submnts, ap->path);

	/* 
	 * EXPIRE_MULTI is synchronous, so we can be sure (famous last
	 * words) the umounts are done by the time we reach here
	 */
	if (count)
		info(ap->logopt, "%d remaining in %s", count, ap->path);

	ec.status = left;

	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &cur_state);
	pthread_cleanup_pop(1);
	pthread_setcancelstate(cur_state, NULL);

	return NULL;
}