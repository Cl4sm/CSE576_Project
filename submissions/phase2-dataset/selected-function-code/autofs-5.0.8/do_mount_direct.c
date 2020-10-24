static void *do_mount_direct(void *arg)
{
	struct ioctl_ops *ops = get_ioctl_ops();
	struct pending_args *args, mt;
	struct autofs_point *ap;
	struct stat st;
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

	pthread_cleanup_push(mount_send_fail, &mt);

	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &state);

	status = fstat(mt.ioctlfd, &st);
	if (status == -1) {
		error(ap->logopt,
		      "can't stat direct mount trigger %s", mt.name);
		ops->send_fail(ap->logopt,
			       mt.ioctlfd, mt.wait_queue_token, -ENOENT);
		ops->close(ap->logopt, mt.ioctlfd);
		pthread_setcancelstate(state, NULL);
		pthread_exit(NULL);
	}

	status = stat(mt.name, &st);
	if (status != 0 || !S_ISDIR(st.st_mode) || st.st_dev != mt.dev) {
		error(ap->logopt,
		     "direct trigger not valid or already mounted %s",
		     mt.name);
		ops->send_ready(ap->logopt, mt.ioctlfd, mt.wait_queue_token);
		ops->close(ap->logopt, mt.ioctlfd);
		pthread_setcancelstate(state, NULL);
		pthread_exit(NULL);
	}

	pthread_setcancelstate(state, NULL);

	info(ap->logopt, "attempting to mount entry %s", mt.name);

	set_tsd_user_vars(ap->logopt, mt.uid, mt.gid);

	status = lookup_nss_mount(ap, NULL, mt.name, mt.len);
	/*
	 * Direct mounts are always a single mount. If it fails there's
	 * nothing to undo so just complain
	 */
	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &state);
	if (status) {
		struct mapent *me;
		struct statfs fs;
		unsigned int close_fd = 0;

		if (statfs(mt.name, &fs) == -1 ||
		   (fs.f_type == AUTOFS_SUPER_MAGIC &&
		    !master_find_submount(ap, mt.name)))
			close_fd = 1;
		cache_writelock(mt.mc);
		if ((me = cache_lookup_distinct(mt.mc, mt.name))) {
			/*
			 * Careful here, we need to leave the file handle open
			 * for direct mount multi-mounts with no real mount at
			 * their base so they will be expired.
			 */
			if (close_fd && me == me->multi)
				close_fd = 0;
			if (!close_fd)
				me->ioctlfd = mt.ioctlfd;
		}
		ops->send_ready(ap->logopt, mt.ioctlfd, mt.wait_queue_token);
		cache_unlock(mt.mc);
		if (close_fd)
			ops->close(ap->logopt, mt.ioctlfd);
		info(ap->logopt, "mounted %s", mt.name);
	} else {
		/* TODO: get mount return status from lookup_nss_mount */
		ops->send_fail(ap->logopt,
			       mt.ioctlfd, mt.wait_queue_token, -ENOENT);
		ops->close(ap->logopt, mt.ioctlfd);
		info(ap->logopt, "failed to mount %s", mt.name);
	}
	pthread_setcancelstate(state, NULL);

	pthread_cleanup_pop(0);

	return NULL;
}