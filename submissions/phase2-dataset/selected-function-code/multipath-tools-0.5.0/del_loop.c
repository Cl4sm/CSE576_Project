extern int 
del_loop (const char *device)
{
	int retries = 3;
	int fd;

	if ((fd = open (device, O_RDONLY)) < 0) {
		int errsv = errno;
		fprintf(stderr, "loop: can't delete device %s: %s\n",
			device, strerror (errsv));
		return 1;
	}

	while (ioctl (fd, LOOP_CLR_FD, 0) < 0) {
		if (errno != EBUSY || retries-- <= 0) {
			perror ("ioctl: LOOP_CLR_FD");
			close (fd);
			return 1;
		}
		fprintf(stderr,
			"loop: device %s still in use, retrying delete\n",
			device);
		sleep(1);
		continue;
	}

	close (fd);
	return 0;
}
