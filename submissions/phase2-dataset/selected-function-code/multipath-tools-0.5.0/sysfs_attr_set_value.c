			     char * value, size_t value_len)
{
	char devpath[PATH_SIZE];
	struct stat statbuf;
	int fd;
	ssize_t size = -1;

	if (!dev || !attr_name || !value || !value_len)
		return 0;

	snprintf(devpath, PATH_SIZE, "%s/%s", udev_device_get_syspath(dev),
		 attr_name);
	condlog(4, "open '%s'", devpath);
	if (stat(devpath, &statbuf) != 0) {
		condlog(4, "stat '%s' failed: %s", devpath, strerror(errno));
		return -errno;
	}

	/* skip directories */
	if (S_ISDIR(statbuf.st_mode)) {
		condlog(4, "%s is a directory", devpath);
		return -EISDIR;
	}

	/* skip non-writeable files */
	if ((statbuf.st_mode & S_IWUSR) == 0) {
		condlog(4, "%s is not writeable", devpath);
		return -EPERM;
	}

	/* write attribute value */
	fd = open(devpath, O_WRONLY);
	if (fd < 0) {
		condlog(4, "attribute '%s' can not be opened: %s",
			devpath, strerror(errno));
		return -errno;
	}
	size = write(fd, value, value_len);
	if (size < 0) {
		condlog(4, "write to %s failed: %s", devpath, strerror(errno));
		size = -errno;
	} else if (size < value_len) {
		condlog(4, "tried to write %ld to %s. Wrote %ld",
			(long)value_len, devpath, (long)size);
		size = 0;
	}

	close(fd);
	return size;
}
