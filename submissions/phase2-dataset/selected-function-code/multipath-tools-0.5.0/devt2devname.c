devt2devname (char *devname, int devname_len, char *devt)
{
	FILE *fd;
	unsigned int tmpmaj, tmpmin, major, minor;
	char dev[FILE_NAME_SIZE];
	char block_path[PATH_SIZE];
	struct stat statbuf;

	memset(block_path, 0, sizeof(block_path));
	memset(dev, 0, sizeof(dev));
	if (sscanf(devt, "%u:%u", &major, &minor) != 2) {
		condlog(0, "Invalid device number %s", devt);
		return 1;
	}

	if (devname_len > FILE_NAME_SIZE)
		devname_len = FILE_NAME_SIZE;

	if (stat("/sys/dev/block", &statbuf) == 0) {
		/* Newer kernels have /sys/dev/block */
		sprintf(block_path,"/sys/dev/block/%u:%u", major, minor);
		if (lstat(block_path, &statbuf) == 0) {
			if (S_ISLNK(statbuf.st_mode) &&
			    readlink(block_path, dev, FILE_NAME_SIZE) > 0) {
				char *p = strrchr(dev, '/');

				if (!p) {
					condlog(0, "No sysfs entry for %s",
						block_path);
					return 1;
				}
				p++;
				strncpy(devname, p, devname_len);
				return 0;
			}
		}
		goto skip_proc;
	}
	memset(block_path, 0, sizeof(block_path));

	if (!(fd = fopen("/proc/partitions", "r"))) {
		condlog(0, "Cannot open /proc/partitions");
		return 1;
	}

	while (!feof(fd)) {
		int r = fscanf(fd,"%u %u %*d %s",&tmpmaj, &tmpmin, dev);
		if (!r) {
			r = fscanf(fd,"%*s\n");
			continue;
		}
		if (r != 3)
			continue;

		if ((major == tmpmaj) && (minor == tmpmin)) {
			if (snprintf(block_path, sizeof(block_path),
				     "/sys/block/%s", dev) >= sizeof(block_path)) {
				condlog(0, "device name %s is too long", dev);
				fclose(fd);
				return 1;
			}
			break;
		}
	}
	fclose(fd);
skip_proc:
	if (strncmp(block_path,"/sys/block", 10)) {
		condlog(3, "No device found for %u:%u", major, minor);
		return 1;
	}

	if (stat(block_path, &statbuf) < 0) {
		condlog(0, "No sysfs entry for %s", block_path);
		return 1;
	}

	if (S_ISDIR(statbuf.st_mode) == 0) {
		condlog(0, "sysfs entry %s is not a directory", block_path);
		return 1;
	}
	basenamecpy((const char *)block_path, devname, devname_len);
	return 0;
}
