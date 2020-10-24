get_hotplug_device(void)
{
	unsigned int major, minor, off, len;
	const char *mapname;
	char *devname = NULL;
	char *device = NULL;
	char *var = NULL;
	struct stat buf;

	var = getenv("ACTION");

	if (!var || strcmp(var, "add"))
		return NULL;

	/* Get dm mapname for hotpluged device. */
	if (!(devname = getenv("DEVNAME")))
		return NULL;

	if (stat(devname, &buf))
		return NULL;

	major = (unsigned int)MAJOR(buf.st_rdev);
	minor = (unsigned int)MINOR(buf.st_rdev);

	if (!(mapname = dm_mapname(major, minor))) /* Not dm device. */
		return NULL;

	off = find_devname_offset(devname);
	len = strlen(mapname);

	/* Dirname + mapname + \0 */
	if (!(device = (char *)malloc(sizeof(char) * (off + len + 1))))
		return NULL;

	/* Create new device name. */
	snprintf(device, off + 1, "%s", devname);
	snprintf(device + off, len + 1, "%s", mapname);

	if (strlen(device) != (off + len))
		return NULL;

	return device;
}
