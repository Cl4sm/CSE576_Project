common_sysfs_pathinfo (struct path * pp)
{
	if (!pp)
		return 1;

	if (!pp->udev) {
		condlog(4, "%s: udev not initialised", pp->dev);
		return 1;
	}
	if (sysfs_get_dev(pp->udev, pp->dev_t, BLK_DEV_SIZE) <= 0) {
		condlog(3, "%s: no 'dev' attribute in sysfs", pp->dev);
		return 1;
	}

	condlog(3, "%s: dev_t = %s", pp->dev, pp->dev_t);

	if (sysfs_get_size(pp, &pp->size))
		return 1;

	condlog(3, "%s: size = %llu", pp->dev, pp->size);

	return 0;
}
