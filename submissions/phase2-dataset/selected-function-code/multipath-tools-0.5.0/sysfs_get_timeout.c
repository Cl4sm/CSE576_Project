int
sysfs_get_timeout(struct path *pp, unsigned int *timeout)
{
	const char *attr = NULL;
	const char *subsys;
	struct udev_device *parent;
	int r;
	unsigned int t;

	if (!pp->udev || pp->bus != SYSFS_BUS_SCSI)
		return -ENOSYS;

	parent = pp->udev;
	while (parent) {
		subsys = udev_device_get_subsystem(parent);
		attr = udev_device_get_sysattr_value(parent, "timeout");
		if (subsys && attr)
			break;
		parent = udev_device_get_parent(parent);
	}
	if (!attr) {
		condlog(3, "%s: No timeout value in sysfs", pp->dev);
		return -ENXIO;
	}

	r = sscanf(attr, "%u\n", &t);

	if (r != 1) {
		condlog(3, "%s: Cannot parse timeout attribute '%s'",
			pp->dev, attr);
		return -EINVAL;
	}

	*timeout = t;

	return 0;
}
