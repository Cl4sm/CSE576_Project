extern int
set_loop (const char *device, const char *file, int offset, int *loopro)
{
	struct loop_info loopinfo;
	int fd, ffd, mode;

	mode = (*loopro ? O_RDONLY : O_RDWR);

	if ((ffd = open (file, mode)) < 0) {

		if (!*loopro && (errno == EROFS || errno == EACCES))
			ffd = open (file, mode = O_RDONLY);

		if (ffd < 0) {
			perror (file);
			return 1;
		}
	}

	if ((fd = open (device, mode)) < 0) {
		perror (device);
		return 1;
	}

	*loopro = (mode == O_RDONLY);
	memset (&loopinfo, 0, sizeof (loopinfo));

	xstrncpy (loopinfo.lo_name, file, LO_NAME_SIZE);
	loopinfo.lo_offset = offset;
	loopinfo.lo_encrypt_type = LO_CRYPT_NONE;
	loopinfo.lo_encrypt_key_size = 0;

	if (ioctl (fd, LOOP_SET_FD, int2ptr(ffd)) < 0) {
		perror ("ioctl: LOOP_SET_FD");
		close (fd);
		close (ffd);
		return 1;
	}

	if (ioctl (fd, LOOP_SET_STATUS, &loopinfo) < 0) {
		(void) ioctl (fd, LOOP_CLR_FD, 0);
		perror ("ioctl: LOOP_SET_STATUS");
		close (fd);
		close (ffd);
		return 1;
	}

	close (fd);
	close (ffd);
	return 0;
}
