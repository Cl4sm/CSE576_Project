int
sysfs_get_size (struct path *pp, unsigned long long * size)
{
	char attr[255];
	int r;

	if (!pp->udev || !size)
		return 1;

	attr[0] = '\0';
	if (sysfs_attr_get_value(pp->udev, "size", attr, 255) == 0) {
		condlog(3, "%s: No size attribute in sysfs", pp->dev);
		return 1;
	}

	r = sscanf(attr, "%llu\n", size);

	if (r != 1) {
		condlog(3, "%s: Cannot parse size attribute", pp->dev);
		*size = 0;
		return 1;
	}

	return 0;
}
