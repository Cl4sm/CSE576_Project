int do_umount_autofs_direct(struct autofs_point *ap, struct mnt_list *mnts, struct mapent *me)
{
	struct ioctl_ops *ops = get_ioctl_ops();
	char buf[MAX_ERR_BUF];
	int ioctlfd = -1, rv, left, retries;
	int opened = 0;

	left = umount_multi(ap, me->key, 0);
	if (left) {
		warn(ap->logopt, "could not unmount %d dirs under %s",
		     left, me->key);
		return 1;
	}

	if (me->ioctlfd != -1) {
		if (tree_is_mounted(mnts, me->key, MNTS_REAL)) {
			error(ap->logopt,
			      "attempt to umount busy direct mount %s",
			      me->key);
			return 1;
		}
		ioctlfd = me->ioctlfd;
	} else {
		ops->open(ap->logopt, &ioctlfd, me->dev, me->key);
		opened = 1;
	}

	if (ioctlfd >= 0) {
		unsigned int status = 1;

		rv = ops->askumount(ap->logopt, ioctlfd, &status);
		if (rv) {
			char *estr = strerror_r(errno, buf, MAX_ERR_BUF);
			error(ap->logopt, "ioctl failed: %s", estr);
			if (opened && ioctlfd != -1)
				ops->close(ap->logopt, ioctlfd);
			return 1;
		} else if (!status) {
			if (ap->state != ST_SHUTDOWN_FORCE) {
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
		error(ap->logopt,
		      "couldn't get ioctl fd for direct mount %s", me->key);
		return 1;
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
		case EINVAL:
			warn(ap->logopt, "mount point %s does not exist",
			      me->key);
			return 0;
			break;
		case EBUSY:
			warn(ap->logopt, "mount point %s is in use", me->key);
			if (ap->state == ST_SHUTDOWN_FORCE)
				goto force_umount;
			else
				return 0;
			break;
		case ENOTDIR:
			error(ap->logopt, "mount point is not a directory");
			return 0;
			break;
		}
		return 1;
	}

force_umount:
	if (rv != 0) {
		info(ap->logopt, "forcing umount of direct mount %s", me->key);
		rv = umount2(me->key, MNT_DETACH);
	} else
		info(ap->logopt, "umounted direct mount %s", me->key);

	if (!rv && me->flags & MOUNT_FLAG_DIR_CREATED) {
		if  (rmdir(me->key) == -1) {
			char *estr = strerror_r(errno, buf, MAX_ERR_BUF);
			warn(ap->logopt, "failed to remove dir %s: %s",
			     me->key, estr);
		}
	}
	return rv;
}