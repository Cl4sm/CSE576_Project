int umount_autofs_offset(struct autofs_point *ap, struct mapent *me)
{
	struct ioctl_ops *ops = get_ioctl_ops();
	char buf[MAX_ERR_BUF];
	int ioctlfd = -1, rv = 1, retries;
	int opened = 0;

	if (me->ioctlfd != -1) {
		if (is_mounted(_PATH_MOUNTED, me->key, MNTS_REAL)) {
			error(ap->logopt,
			      "attempt to umount busy offset %s", me->key);
			return 1;
		}
		ioctlfd = me->ioctlfd;
	} else {
		/* offset isn't mounted, return success and try to recover */
		if (!is_mounted(_PROC_MOUNTS, me->key, MNTS_AUTOFS)) {
			debug(ap->logopt,
			      "offset %s not mounted",
			      me->key);
			return 0;
		}
		ops->open(ap->logopt, &ioctlfd, me->dev, me->key);
		opened = 1;
	}

	if (ioctlfd >= 0) {
		unsigned int status = 1;

		rv = ops->askumount(ap->logopt, ioctlfd, &status);
		if (rv) {
			char *estr = strerror_r(errno, buf, MAX_ERR_BUF);
			logerr("ioctl failed: %s", estr);
			if (opened && ioctlfd != -1)
				ops->close(ap->logopt, ioctlfd);
			return 1;
		} else if (!status) {
			if (ap->state != ST_SHUTDOWN_FORCE) {
				if (ap->shutdown)
					error(ap->logopt,
					     "ask umount returned busy for %s",
					     me->key);
				if (opened && ioctlfd != -1)
					ops->close(ap->logopt, ioctlfd);
				return 1;
			} else {
				me->ioctlfd = -1;
				ops->catatonic(ap->logopt, ioctlfd);
				ops->close(ap->logopt, ioctlfd);
				goto force_umount;
			}
		}
		me->ioctlfd = -1;
		ops->catatonic(ap->logopt, ioctlfd);
		ops->close(ap->logopt, ioctlfd);
	} else {
		struct stat st;
		char *estr;
		int save_errno = errno;

		/* Non existent directory on remote fs - no mount */
		if (stat(me->key, &st) == -1 && errno == ENOENT)
			return 0;

		estr = strerror_r(save_errno, buf, MAX_ERR_BUF);
		error(ap->logopt,
		      "couldn't get ioctl fd for offset %s: %s",
		      me->key, estr);
		goto force_umount;
	}

	sched_yield();

	retries = UMOUNT_RETRIES;
	while ((rv = umount(me->key)) == -1 && retries--) {
		struct timespec tm = {0, 200000000};
		if (errno != EBUSY)
			break;
		nanosleep(&tm, NULL);
	}

	if (rv == -1) {
		switch (errno) {
		case ENOENT:
			warn(ap->logopt, "mount point does not exist");
			return 0;
			break;
		case EBUSY:
			error(ap->logopt, "mount point %s is in use", me->key);
			if (ap->state != ST_SHUTDOWN_FORCE)
				return 1;
			break;
		case ENOTDIR:
			error(ap->logopt, "mount point is not a directory");
			return 0;
			break;
		}
		goto force_umount;
	}

force_umount:
	if (rv != 0) {
		info(ap->logopt, "forcing umount of offset mount %s", me->key);
		rv = umount2(me->key, MNT_DETACH);
	} else
		info(ap->logopt, "umounted offset mount %s", me->key);

	return rv;
}