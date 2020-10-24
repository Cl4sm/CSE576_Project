int umount_autofs_indirect(struct autofs_point *ap, const char *root)
{
	struct ioctl_ops *ops = get_ioctl_ops();
	char buf[MAX_ERR_BUF];
	char mountpoint[PATH_MAX + 1];
	int rv, retries;
	unsigned int unused;

	if (root)
		strcpy(mountpoint, root);
	else
		strcpy(mountpoint, ap->path);

	/* If we are trying to shutdown make sure we can umount */
	rv = ops->askumount(ap->logopt, ap->ioctlfd, &unused);
	if (rv == -1) {
		char *estr = strerror_r(errno, buf, MAX_ERR_BUF);
		logerr("ioctl failed: %s", estr);
		return 1;
	} else if (!unused) {
#if defined(ENABLE_IGNORE_BUSY_MOUNTS) || defined(ENABLE_FORCED_SHUTDOWN)
		if (!ap->shutdown)
			return 1;
		error(ap->logopt, "ask umount returned busy %s", ap->path);
#else
		return 1;
#endif
	}

	if (ap->shutdown)
		ops->catatonic(ap->logopt, ap->ioctlfd);

	ops->close(ap->logopt, ap->ioctlfd);
	ap->ioctlfd = -1;
	sched_yield();

	retries = UMOUNT_RETRIES;
	while ((rv = umount(mountpoint)) == -1 && retries--) {
		struct timespec tm = {0, 200000000};
		if (errno != EBUSY)
			break;
		nanosleep(&tm, NULL);
	}

	if (rv == -1) {
		switch (errno) {
		case ENOENT:
		case EINVAL:
			error(ap->logopt,
			      "mount point %s does not exist", mountpoint);
			close_mount_fds(ap);
			return 0;
			break;
		case EBUSY:
			debug(ap->logopt,
			      "mount point %s is in use", mountpoint);
			if (ap->state == ST_SHUTDOWN_FORCE) {
				close_mount_fds(ap);
				goto force_umount;
			} else {
				/*
				 * If the umount returns EBUSY there may be
				 * a mount request in progress so we need to
				 * recover unless we have been explicitly
				 * asked to shutdown and configure option
				 * ENABLE_IGNORE_BUSY_MOUNTS is enabled.
				 */
#ifdef ENABLE_IGNORE_BUSY_MOUNTS
				if (ap->shutdown) {
					close_mount_fds(ap);
					return 0;
				}
#endif
				ops->open(ap->logopt,
					  &ap->ioctlfd, ap->dev, mountpoint);
				if (ap->ioctlfd < 0) {
					warn(ap->logopt,
					     "could not recover autofs path %s",
					     mountpoint);
					close_mount_fds(ap);
					return 0;
				}
			}
			break;
		case ENOTDIR:
			error(ap->logopt, "mount point is not a directory");
			close_mount_fds(ap);
			return 0;
			break;
		}
		return 1;
	}

	/*
	 * We have successfully umounted the mount so we now close
	 * the descriptors. The kernel end of the kernel pipe will
	 * have been put during the umount super block cleanup.
	 */
	close_mount_fds(ap);

force_umount:
	if (rv != 0) {
		warn(ap->logopt,
		     "forcing umount of indirect mount %s", mountpoint);
		rv = umount2(mountpoint, MNT_DETACH);
	} else {
		info(ap->logopt, "umounted indirect mount %s", mountpoint);
		if (ap->submount)
			rm_unwanted(ap->logopt, mountpoint, 1, ap->dev);
	}

	return rv;
}