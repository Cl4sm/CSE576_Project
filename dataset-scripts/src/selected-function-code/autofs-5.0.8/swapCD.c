int swapCD(const char *device, const char *slotName)
{
	int fd;			/* file descriptor for CD-ROM device */
	int status;		/* return status for system calls */
	int slot = -1;
	int total_slots_available;

	slot = atoi(slotName) - 1;

	/* open device */
	fd = open_fd(device, O_RDONLY | O_NONBLOCK);
	if (fd < 0) {
		logerr(MODPREFIX "Opening device %s failed : %s",
		      device, strerror(errno));
		return 1;
	}

	/* Check CD player status */
	total_slots_available = ioctl(fd, CDROM_CHANGER_NSLOTS);
	if (total_slots_available <= 1) {
		logerr(MODPREFIX
		      "Device %s is not an ATAPI compliant CD changer.",
		      device);
		close(fd);
		return 1;
	}

	/* load */
	slot = ioctl(fd, CDROM_SELECT_DISC, slot);
	if (slot < 0) {
		logerr(MODPREFIX "CDROM_SELECT_DISC failed");
		close(fd);
		return 1;
	}

	/* close device */
	status = close(fd);
	if (status != 0) {
		logerr(MODPREFIX "close failed for `%s': %s",
		      device, strerror(errno));
		return 1;
	}
	return 0;
}