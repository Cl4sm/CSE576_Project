static void *do_expire_direct(void *arg)
{
	struct ioctl_ops *ops = get_ioctl_ops();
	struct pending_args *args, mt;
	struct autofs_point *ap;
	size_t len;
	int status, state;

	args = (struct pending_args *) arg;

	pending_mutex_lock(args);

	memcpy(&mt, args, sizeof(struct pending_args));

	ap = mt.ap;

	args->signaled = 1;
	status = pthread_cond_signal(&args->cond);
	if (status)
		fatal(status);

	pending_mutex_unlock(args);

	pthread_cleanup_push(expire_send_fail, &mt);

	len = _strlen(mt.name, KEY_MAX_LEN);
	if (!len) {
		warn(ap->logopt, "direct key path too long %s", mt.name);
		/* TODO: force umount ?? */
		pthread_exit(NULL);
	}

	status = do_expire(ap, mt.name, len);
	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &state);
	if (status)
		ops->send_fail(ap->logopt,
			       mt.ioctlfd, mt.wait_queue_token, -ENOENT);
	else {
		struct mapent *me;
		cache_writelock(mt.mc);
		me = cache_lookup_distinct(mt.mc, mt.name);
		if (me)
			me->ioctlfd = -1;
		cache_unlock(mt.mc);
		ops->send_ready(ap->logopt, mt.ioctlfd, mt.wait_queue_token);
		ops->close(ap->logopt, mt.ioctlfd);
	}
	pthread_setcancelstate(state, NULL);

	pthread_cleanup_pop(0);

	return NULL;
}