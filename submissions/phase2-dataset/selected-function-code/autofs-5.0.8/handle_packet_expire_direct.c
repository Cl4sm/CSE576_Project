int handle_packet_expire_direct(struct autofs_point *ap, autofs_packet_expire_direct_t *pkt)
{
	struct ioctl_ops *ops = get_ioctl_ops();
	struct map_source *map;
	struct mapent_cache *mc = NULL;
	struct mapent *me = NULL;
	struct pending_args *mt;
	char buf[MAX_ERR_BUF];
	pthread_t thid;
	struct timespec wait;
	struct timeval now;
	int status, state;

	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &state);

	/*
	 * This is a bit of a big deal.
	 * If we can't find the path and the map entry then
	 * we can't send a notification back to the kernel.
	 * Hang results.
	 *
	 * OTOH there is a mount so there should be a path
	 * and since it got mounted we have to trust that
	 * there is an entry in the cache.
	 */
	master_source_writelock(ap->entry);
	map = ap->entry->maps;
	while (map) {
		mc = map->mc;
		cache_writelock(mc);
		me = cache_lookup_ino(mc, pkt->dev, pkt->ino);
		if (me)
			break;
		cache_unlock(mc);
		map = map->next;
	}

	if (!me) {
		/*
		 * Shouldn't happen as we have been sent this following
		 * successful thread creation and lookup.
		 */
		crit(ap->logopt, "can't find map entry for (%lu,%lu)",
		    (unsigned long) pkt->dev, (unsigned long) pkt->ino);
		master_source_unlock(ap->entry);
		pthread_setcancelstate(state, NULL);
		return 1;
	}

	/* Can't expire it if it isn't mounted */
	if (me->ioctlfd == -1) {
		int ioctlfd;
		ops->open(ap->logopt, &ioctlfd, me->dev, me->key);
		if (ioctlfd == -1) {
			crit(ap->logopt, "can't open ioctlfd for %s", me->key);
			cache_unlock(mc);
			master_source_unlock(ap->entry);
			pthread_setcancelstate(state, NULL);
			return 1;
		}
		ops->send_ready(ap->logopt, ioctlfd, pkt->wait_queue_token);
		ops->close(ap->logopt, ioctlfd);
		cache_unlock(mc);
		master_source_unlock(ap->entry);
		pthread_setcancelstate(state, NULL);
		return 0;
	}

	mt = malloc(sizeof(struct pending_args));
	if (!mt) {
		char *estr = strerror_r(errno, buf, MAX_ERR_BUF);
		error(ap->logopt, "malloc: %s", estr);
		ops->send_fail(ap->logopt,
			       me->ioctlfd, pkt->wait_queue_token, -ENOMEM);
		cache_unlock(mc);
		master_source_unlock(ap->entry);
		pthread_setcancelstate(state, NULL);
		return 1;
	}

	status = pthread_cond_init(&mt->cond, NULL);
	if (status)
		fatal(status);

	status = pthread_mutex_init(&mt->mutex, NULL);
	if (status)
		fatal(status);

	mt->ap = ap;
	mt->ioctlfd = me->ioctlfd;
	mt->mc = mc;
	/* TODO: check length here */
	strcpy(mt->name, me->key);
	mt->dev = me->dev;
	mt->type = NFY_EXPIRE;
	mt->wait_queue_token = pkt->wait_queue_token;

	debug(ap->logopt, "token %ld, name %s",
		  (unsigned long) pkt->wait_queue_token, mt->name);

	pending_mutex_lock(mt);

	status = pthread_create(&thid, &th_attr_detached, do_expire_direct, mt);
	if (status) {
		error(ap->logopt, "expire thread create failed");
		ops->send_fail(ap->logopt,
			       mt->ioctlfd, pkt->wait_queue_token, -status);
		cache_unlock(mc);
		master_source_unlock(ap->entry);
		pending_mutex_unlock(mt);
		pending_cond_destroy(mt);
		pending_mutex_destroy(mt);
		free_pending_args(mt);
		pthread_setcancelstate(state, NULL);
		return 1;
	}

	cache_unlock(mc);
	master_source_unlock(ap->entry);

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