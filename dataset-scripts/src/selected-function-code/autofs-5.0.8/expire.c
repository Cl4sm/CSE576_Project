static int expire(unsigned int logopt,
		  int cmd, int fd, int ioctlfd, const char *path, void *arg)
{
	int ret, retries = EXPIRE_RETRIES;
	unsigned int may_umount;

	while (retries--) {
		struct timespec tm = {0, 100000000};

		/* Ggenerate expire message for the mount. */
		ret = ioctl(fd, cmd, arg);
		if (ret == -1) {
			/* Mount has gone away */
			if (errno == EBADF || errno == EINVAL)
				return 0;

			/*
			 * Other than EAGAIN is an expire error so continue.
			 * Kernel will try the next mount for indirect maps
			 * and the same mount again for direct maps, limited
			 * by retries.
			 */
			if (errno == EAGAIN)
				break;
		}
		nanosleep(&tm, NULL);
	}

	may_umount = 0;
	if (ctl.ops->askumount(logopt, ioctlfd, &may_umount))
		return -1;

	if (!may_umount)
		return 1;

	return 0;
}