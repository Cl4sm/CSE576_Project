void init_ioctl_ctl(void)
{
	int devfd;

	if (ctl.ops)
		return;

	devfd = open_fd(CONTROL_DEVICE, O_RDONLY);
	if (devfd == -1)
		ctl.ops = &ioctl_ops;
	else {
		struct autofs_dev_ioctl param;

		/*
		 * Check compile version against kernel.
		 * Selinux may allow us to open the device but not
		 * actually allow us to do anything.
		 */
		init_autofs_dev_ioctl(&param);
		if (ioctl(devfd, AUTOFS_DEV_IOCTL_VERSION, &param) == -1) {
			close(devfd);
			ctl.ops = &ioctl_ops;
		} else {
			ctl.devfd = devfd;
			ctl.ops = &dev_ioctl_ops;
		}
	}
	return;
}