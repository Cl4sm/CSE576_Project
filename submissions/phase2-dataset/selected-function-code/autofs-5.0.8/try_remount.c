int try_remount(struct autofs_point *ap, struct mapent *me, unsigned int type)
{
	struct ioctl_ops *ops = get_ioctl_ops();
	const char *path;
	int ret, fd;
	dev_t devid;

	if (type == t_indirect)
		path = ap->path;
	else
		path = me->key;

	ret = ops->mount_device(ap->logopt, path, type, &devid);
	if (ret == -1 || ret == 0)
		return -1;

	ret = remount_active_mount(ap, me, path, devid, type, &fd);

	/*
	 * The directory must exist since we found a device
	 * number for the mount but we can't know if we created
	 * it or not. However, if this is an indirect mount with
	 * the nobrowse option we need to remove the mount point
	 * directory at umount anyway.
	 */
	if (type == t_indirect) {
		if (ap->flags & MOUNT_FLAG_GHOST)
			ap->flags &= ~MOUNT_FLAG_DIR_CREATED;
		else
			ap->flags |= MOUNT_FLAG_DIR_CREATED;
	} else
		me->flags &= ~MOUNT_FLAG_DIR_CREATED;

	/*
	 * Either we opened the mount or we're re-reading the map.
	 * If we opened the mount and ioctlfd is not -1 we have
	 * a descriptor for the indirect mount so we need to
	 * record that in the mount point struct. Otherwise we're
	 * re-reading the map.
	*/
	if (ret == REMOUNT_READ_MAP)
		return 1;
	else if (ret == REMOUNT_SUCCESS) {
		if (fd != -1) {
			if (type == t_indirect)
				ap->ioctlfd = fd;
			else
				me->ioctlfd = fd;
			return 1;
		}

		/* Indirect mount requires a valid fd */
		if (type != t_indirect)
			return 1;
	}

	/*
	 * Since we got the device number above a mount exists so
	 * any other failure warrants a failure return here.
	 */
	return 0;
}