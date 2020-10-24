static int dev_ioctl_ismountpoint(unsigned int logopt,
				  int ioctlfd, const char *path,
				  unsigned int *mountpoint)
{
	struct autofs_dev_ioctl *param;
	int err;

	*mountpoint = 0;

	if (!path) {
		errno = EINVAL;
		return -1;
	}

	param = alloc_dev_ioctl_path(ioctlfd, path);
	if (!param)
		return -1;
	set_autofs_type_any(&param->ismountpoint.in.type);

	err = ioctl(ctl.devfd, AUTOFS_DEV_IOCTL_ISMOUNTPOINT, param);
	if (err == -1) {
		int save_errno = errno;
		free_dev_ioctl_path(param);
		errno = save_errno;
		return -1;
	}

	if (err) {
		*mountpoint = DEV_IOCTL_IS_MOUNTED;

		if (param->ismountpoint.out.magic == AUTOFS_SUPER_MAGIC)
			*mountpoint |= DEV_IOCTL_IS_AUTOFS;
		else
			*mountpoint |= DEV_IOCTL_IS_OTHER;
	}

	free_dev_ioctl_path(param);

	return 0;
}