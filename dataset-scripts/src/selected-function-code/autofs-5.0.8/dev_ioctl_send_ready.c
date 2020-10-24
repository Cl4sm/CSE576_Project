static int dev_ioctl_send_ready(unsigned int logopt,
				int ioctlfd, unsigned int token)
{
	struct autofs_dev_ioctl param;

	if (token == 0) {
		errno = EINVAL;
		return -1;
	}

	debug(logopt, "token = %d", token);

	init_autofs_dev_ioctl(&param);
	param.ioctlfd = ioctlfd;
	param.ready.token = token;

	if (ioctl(ctl.devfd, AUTOFS_DEV_IOCTL_READY, &param) == -1) {
		char *estr, buf[MAX_ERR_BUF];
		int save_errno = errno;
		estr = strerror_r(errno, buf, MAX_ERR_BUF);
		logerr("AUTOFS_DEV_IOCTL_READY: error %s", estr);
		errno = save_errno;
		return -1;
	}
	return 0;
}