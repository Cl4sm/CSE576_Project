int handle_packet_missing_indirect(struct autofs_point *ap, autofs_packet_missing_indirect_t *pkt)
{
	struct ioctl_ops *ops = get_ioctl_ops();
	pthread_t thid;
	char buf[MAX_ERR_BUF];
	struct pending_args *mt;
	struct timespec wait;
	struct timeval now;
	struct mapent *me;
	int status, state;

	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &state);

	master_mutex_lock();

	debug(ap->logopt, "token %ld, name %s, request pid %u",
		(unsigned long) pkt->wait_queue_token, pkt->name, pkt->pid);

	/* Ignore packet if we're trying to shut down */
	if (ap->shutdown || ap->state == ST_SHUTDOWN_FORCE) {
		ops->send_fail(ap->logopt,
			       ap->ioctlfd, pkt->wait_queue_token, -ENOENT);
		master_mutex_unlock();
		pthread_setcancelstate(state, NULL);
		return 0;
	}

	/* Check if we recorded a mount fail for this key anywhere */
	me = lookup_source_mapent(ap, pkt->name, LKP_DISTINCT);
	if (me) {
		if (me->status >= time(NULL)) {
			ops->send_fail(ap->logopt, ap->ioctlfd,
				       pkt->wait_queue_token, -ENOENT);
			cache_unlock(me->mc);
			master_mutex_unlock();
			pthread_setcancelstate(state, NULL);
			return 0;
		}
		cache_unlock(me->mc);
	}

	mt = malloc(sizeof(struct pending_args));
	if (!mt) {
		char *estr = strerror_r(errno, buf, MAX_ERR_BUF);
		logerr("malloc: %s", estr);
		ops->send_fail(ap->logopt,
			       ap->ioctlfd, pkt->wait_queue_token, -ENOMEM);
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
	strncpy(mt->name, pkt->name, pkt->len);
	mt->name[pkt->len] = '\0';
	mt->len = pkt->len;
	mt->dev = pkt->dev;
	mt->uid = pkt->uid;
	mt->gid = pkt->gid;
	mt->wait_queue_token = pkt->wait_queue_token;

	status = pthread_create(&thid, &th_attr_detached, do_mount_indirect, mt);
	if (status) {
		error(ap->logopt, "expire thread create failed");
		ops->send_fail(ap->logopt,
			       ap->ioctlfd, pkt->wait_queue_token, -status);
		master_mutex_unlock();
		pending_mutex_unlock(mt);
		pending_cond_destroy(mt);
		pending_mutex_destroy(mt);
		free_pending_args(mt);
		pthread_setcancelstate(state, NULL);
		return 1;
	}

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