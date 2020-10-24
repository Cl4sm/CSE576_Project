static int ioctl_open(unsigned int logopt,
		      int *ioctlfd, dev_t devid, const char *path)
{
	struct statfs sfs;
	int save_errno, fd;

	*ioctlfd = -1;

	fd = open_fd(path, O_RDONLY);
	if (fd == -1)
		return -1;

	if (fstatfs(fd, &sfs) == -1) {
		save_errno = errno;
		goto err;
	}

	if (sfs.f_type != AUTOFS_SUPER_MAGIC) {
		save_errno = ENOENT;
		goto err;
	}

	*ioctlfd = fd;

	return 0;
err:
	close(fd);
	errno = save_errno;
	return -1;
}