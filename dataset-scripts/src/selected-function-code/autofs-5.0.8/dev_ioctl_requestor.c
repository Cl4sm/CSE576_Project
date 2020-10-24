static int dev_ioctl_requestor(unsigned int logopt,
			       int ioctlfd, const char *path,
			       uid_t *uid, gid_t *gid)
{
	struct autofs_dev_ioctl *param;
	int err;

	if (!path)
		errno = EINVAL;

	*uid = -1;
	*gid = -1;


	param = alloc_dev_ioctl_path(ioctlfd, path);
	if (!param)
		return -1;

	err = ioctl(ctl.devfd, AUTOFS_DEV_IOCTL_REQUESTER, param);
	if (err == -1) {
		int save_errno = errno;
		free_dev_ioctl_open(param);
		errno = save_errno;
		return -1;
	}

	*uid = param->requester.uid;
	*gid = param->requester.gid;

	free_dev_ioctl_path(param);

	return 0;
}