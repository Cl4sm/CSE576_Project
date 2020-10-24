path_offline (struct path * pp)
{
	struct udev_device * parent;
	char buff[SCSI_STATE_SIZE];
	int err;

	if (pp->bus != SYSFS_BUS_SCSI)
		return PATH_UP;

	parent = pp->udev;
	while (parent) {
		const char *subsys = udev_device_get_subsystem(parent);
		if (subsys && !strncmp(subsys, "scsi", 4))
			break;
		parent = udev_device_get_parent(parent);
	}

	if (!parent) {
		condlog(1, "%s: failed to get sysfs information", pp->dev);
		return PATH_REMOVED;
	}

	memset(buff, 0x0, SCSI_STATE_SIZE);
	err = sysfs_attr_get_value(parent, "state", buff, SCSI_STATE_SIZE);
	if (err <= 0) {
		if (err == -ENXIO)
			return PATH_REMOVED;
		else
			return PATH_DOWN;
	}


	condlog(3, "%s: path state = %s", pp->dev, buff);

	if (!strncmp(buff, "offline", 7) ||
	    !strncmp(buff, "quiesce", 7) ||
	    !strncmp(buff, "transport-offline", 17)) {
		pp->offline = 1;
		return PATH_DOWN;
	}
	pp->offline = 0;
	if (!strncmp(buff, "blocked", 7) || !strncmp(buff, "quiesce", 7))
		return PATH_PENDING;
	else if (!strncmp(buff, "running", 7))
		return PATH_UP;

	return PATH_DOWN;
}
