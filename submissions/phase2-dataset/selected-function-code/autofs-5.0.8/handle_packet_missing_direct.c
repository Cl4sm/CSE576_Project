int handle_packet_missing_direct(struct autofs_point *ap, autofs_packet_missing_direct_t *pkt)
{
	struct ioctl_ops *ops = get_ioctl_ops();
	struct map_source *map;
	struct mapent_cache *mc = NULL;
	struct mapent *me = NULL;
	pthread_t thid;
	struct pending_args *mt;
	char buf[MAX_ERR_BUF];
	int status = 0;
	struct timespec wait;
	struct timeval now;
	int ioctlfd, len, state;
	unsigned int kver_major = get_kver_major();
	unsigned int kver_minor = get_kver_minor();

	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &state);

	master_mutex_lock();

	/*
	 * If our parent is a direct or offset mount that has been
	 * covered by a mount and another lookup occurs after the
	 * mount but before the device and inode are set in the
	 * cache entry we will not be able to find the mapent. So
	 * we must take the source writelock to ensure the parent
	 * has mount is complete before we look for the entry.
	 *
	 * Since the vfs-automount kernel changes we can now block
	 * on covered mounts during mount tree construction so a
	 * write lock is no longer needed. So we now can handle a
	 * wider class of recursively define mount lookups.
	 */
	if (kver_major > 5 || (kver_major == 5 && kver_minor > 1))
		master_source_readlock(ap->entry);
	else
		master_source_writelock(ap->entry);
	map = ap->entry->maps;
	while (map) {
		/*
		 * Only consider map sources that have been read since
		 * the map entry was last updated.
		 */
		if (ap->entry->age > map->age) {
			map = map->next;
			continue;
		}

		mc = map->mc;
		cache_readlock(mc);
		me = cache_lookup_ino(mc, pkt->dev, pkt->ino);
		if (me)
			break;
		cache_unlock(mc);
		map = map->next;
	}

	if (!me) {
		/*
		 * Shouldn't happen as the kernel is telling us
		 * someone has walked on our mount point.
		 */
		logerr("can't find map entry for (%lu,%lu)",
		    (unsigned long) pkt->dev, (unsigned long) pkt->ino);
		master_source_unlock(ap->entry);
		master_mutex_unlock();
		pthread_setcancelstate(state, NULL);
		return 1;
	}

	if (me->ioctlfd != -1) {
		/* Maybe someone did a manual umount, clean up ! */
		close(me->ioctlfd);
		me->ioctlfd = -1;
	}
	ops->open(ap->logopt, &ioctlfd, me->dev, me->key);

	if (ioctlfd == -1) {
		cache_unlock(mc);
		master_source_unlock(ap->entry);
		master_mutex_unlock();
		pthread_setcancelstate(state, NULL);
		crit(ap->logopt, "failed to create ioctl fd for %s", me->key);
		/* TODO:  how do we clear wait q in kernel ?? */
		return 1;
	}

	debug(ap->logopt, "token %ld, name %s, request pid %u",
		  (unsigned long) pkt->wait_queue_token, me->key, pkt->pid);

	/* Ignore packet if we're trying to shut down */
	if (ap->shutdown || ap->state == ST_SHUTDOWN_FORCE) {
		ops->send_fail(ap->logopt,
			       ioctlfd, pkt->wait_queue_token, -ENOENT);
		ops->close(ap->logopt, ioctlfd);
		cache_unlock(mc);
		master_source_unlock(ap->entry);
		master_mutex_unlock();
		pthread_setcancelstate(state, NULL);
		return 1;
	}

	/* Check if we recorded a mount fail for this key */
	if (me->status >= time(NULL)) {
		ops->send_fail(ap->logopt,
			       ioctlfd, pkt->wait_queue_token, -ENOENT);
		ops->close(ap->logopt, ioctlfd);
		cache_unlock(mc);
		master_source_unlock(ap->entry);
		master_mutex_unlock();
		pthread_setcancelstate(state, NULL);
		return 0;
	}

	len = strlen(me->key);
	if (len >= PATH_MAX) {
		error(ap->logopt, "direct mount path too long %s", me->key);
		ops->send_fail(ap->logopt,
			       ioctlfd, pkt->wait_queue_token, -ENAMETOOLONG);
		ops->close(ap->logopt, ioctlfd);
		cache_unlock(mc);
		master_source_unlock(ap->entry);
		master_mutex_unlock();
		pthread_setcancelstate(state, NULL);
		return 1;
	}

	mt = malloc(sizeof(struct pending_args));
	if (!mt) {
		char *estr = strerror_r(errno, buf, MAX_ERR_BUF);
		error(ap->logopt, "malloc: %s", estr);
		ops->send_fail(ap->logopt,
			       ioctlfd, pkt->wait_queue_token, -ENOMEM);
		ops->close(ap->logopt, ioctlfd);
		cache_unlock(mc);
		master_source_unlock(ap->entry);
		master_mutex_unlock();
		pthread_setcancelstate(state, NULL);
		return 1;
	}
	memset(mt, 0, sizeof(struct pending_args));

	status = pthread_cond_init(&mt->cond, NULL);
	if (status)
		fatal(status);

	status = pthread_mutex_init(&mt->mutex, NULL);
	if (status)
		fatal(status);

	pending_mutex_lock(mt);

	mt->ap = ap;
	mt->ioctlfd = ioctlfd;
	mt->mc = mc;
	strcpy(mt->name, me->key);
	mt->len = len;
	mt->dev = me->dev;
	mt->type = NFY_MOUNT;
	mt->uid = pkt->uid;
	mt->gid = pkt->gid;
	mt->wait_queue_token = pkt->wait_queue_token;

	status = pthread_create(&thid, &th_attr_detached, do_mount_direct, mt);
	if (status) {
		error(ap->logopt, "missing mount thread create failed");
		ops->send_fail(ap->logopt,
			       ioctlfd, pkt->wait_queue_token, -status);
		ops->close(ap->logopt, ioctlfd);
		cache_unlock(mc);
		master_source_unlock(ap->entry);
		master_mutex_unlock();
		pending_mutex_unlock(mt);
		pending_cond_destroy(mt);
		pending_mutex_destroy(mt);
		free_pending_args(mt);
		pthread_setcancelstate(state, NULL);
		return 1;
	}

	cache_unlock(mc);
	master_source_unlock(ap->entry);

	master_mutex_unlock();

	pthread_cleanup_push(free_pending_args, mt);
	pthread_cleanup_push(pending_mutex_destroy, mt);
	pthread_cleanup_push(pending_cond_destroy, mt);
	pthread_cleanup_push(pending_mutex_unlock, mt);
	pthread_setcancelstate(state, NULL);

	mt->signaled = 0;
	while (!mt->signaled) {
		gettimeofday(&now, NULL);
		wait.tv_sec = now.tv_sec + 2;
		wait.tv_nsec = now.tv_usec * 1000;
		status = pthread_cond_timedwait(&mt->cond, &mt->mutex, &wait);
		if (status && status != ETIMEDOUT)
			fatal(status);
	}

	pthread_cleanup_pop(1);
	pthread_cleanup_pop(1);
	pthread_cleanup_pop(1);
	pthread_cleanup_pop(1);

	return 0;
}