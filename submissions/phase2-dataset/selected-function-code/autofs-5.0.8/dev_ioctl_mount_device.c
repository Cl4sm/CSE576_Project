static int dev_ioctl_mount_device(unsigned int logopt, const char *path, unsigned int type, dev_t *devid)
{
	struct autofs_dev_ioctl *param;
	int err;

	if (!path) {
		errno = EINVAL;
		return -1;
	}

	*devid = -1;

	param = alloc_dev_ioctl_path(-1, path);
	if (!param)
		return -1;
	param->ismountpoint.in.type = type;

	err = ioctl(ctl.devfd, AUTOFS_DEV_IOCTL_ISMOUNTPOINT, param);
	if (err == -1) {
		int save_errno = errno;
		free_dev_ioctl_path(param);
		errno = save_errno;
		return -1;
	}

	if (err)
		*devid = param->ismountpoint.out.devid;

	free_dev_ioctl_path(param);

	return err;
}