			     char * value, size_t value_len)
{
	char devpath[PATH_SIZE];
	struct stat statbuf;
	int fd;
	ssize_t size = -1;

	if (!dev || !attr_name || !value)
		return 0;

	snprintf(devpath, PATH_SIZE, "%s/%s", udev_device_get_syspath(dev),
		 attr_name);
	condlog(4, "open '%s'", devpath);
	if (stat(devpath, &statbuf) != 0) {
		condlog(4, "stat '%s' failed: %s", devpath, strerror(errno));
		return -ENXIO;
	}

	/* skip directories */
	if (S_ISDIR(statbuf.st_mode)) {
		condlog(4, "%s is a directory", devpath);
		return -EISDIR;
	}

	/* skip non-writeable files */
	if ((statbuf.st_mode & S_IRUSR) == 0) {
		condlog(4, "%s is not readable", devpath);
		return -EPERM;
	}

	/* read attribute value */
	fd = open(devpath, O_RDONLY);
	if (fd < 0) {
		condlog(4, "attribute '%s' can not be opened: %s",
			devpath, strerror(errno));
		return -errno;
	}
	size = read(fd, value, value_len);
	if (size < 0) {
		condlog(4, "read from %s failed: %s", devpath, strerror(errno));
		size = -errno;
	} else if (size == value_len) {
		condlog(4, "overflow while reading from %s", devpath);
		size = 0;
	}

	close(fd);
	return size;
}
