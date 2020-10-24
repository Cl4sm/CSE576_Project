static int dev_ioctl_open(unsigned int logopt,
			  int *ioctlfd, dev_t devid, const char *path)
{
	struct autofs_dev_ioctl *param;

	*ioctlfd = -1;

	param = alloc_dev_ioctl_open(path, devid);
	if (!param)
		return -1;

	if (ioctl(ctl.devfd, AUTOFS_DEV_IOCTL_OPENMOUNT, param) == -1) {
		int save_errno = errno;
		free_dev_ioctl_open(param);
		errno = save_errno;
		return -1;
	}

	*ioctlfd = param->ioctlfd;

	free_dev_ioctl_open(param);

	return 0;
}