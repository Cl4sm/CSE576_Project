static void *do_mount_indirect(void *arg)
{
	struct ioctl_ops *ops = get_ioctl_ops();
	struct pending_args *args, mt;
	struct autofs_point *ap;
	char buf[PATH_MAX + 1];
	struct stat st;
	int len, status, state;

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

	len = ncat_path(buf, sizeof(buf), ap->path, mt.name, mt.len);
	if (!len) {
		crit(ap->logopt, "path to be mounted is to long");
		ops->send_fail(ap->logopt,
			       ap->ioctlfd, mt.wait_queue_token,
			      -ENAMETOOLONG);
		pthread_setcancelstate(state, NULL);
		pthread_exit(NULL);
	}

	status = lstat(buf, &st);
	if (status != -1 && !(S_ISDIR(st.st_mode) && st.st_dev == mt.dev)) {
		error(ap->logopt,
		      "indirect trigger not valid or already mounted %s", buf);
		ops->send_ready(ap->logopt, ap->ioctlfd, mt.wait_queue_token);
		pthread_setcancelstate(state, NULL);
		pthread_exit(NULL);
	}

	pthread_setcancelstate(state, NULL);

	info(ap->logopt, "attempting to mount entry %s", buf);

	set_tsd_user_vars(ap->logopt, mt.uid, mt.gid);

	status = lookup_nss_mount(ap, NULL, mt.name, mt.len);
	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &state);
	if (status) {
		ops->send_ready(ap->logopt,
				ap->ioctlfd, mt.wait_queue_token);
		info(ap->logopt, "mounted %s", buf);
	} else {
		/* TODO: get mount return status from lookup_nss_mount */
		ops->send_fail(ap->logopt,
			       ap->ioctlfd, mt.wait_queue_token, -ENOENT);
		info(ap->logopt, "failed to mount %s", buf);
	}
	pthread_setcancelstate(state, NULL);

	pthread_cleanup_pop(0);

	return NULL;
}