int aquire_flag_file(void)
{
	char linkf[PATH_MAX];
	size_t len;

	len = snprintf(linkf, sizeof(linkf), "%s.%d", FLAG_FILE, getpid());
	if (len >= sizeof(linkf))
		/* Didn't acquire it */
		return 0;

	/*
	 * Repeat until it was us who made the link or we find the
	 * flag file already exists. If an unexpected error occurs
	 * we return 0 claiming the flag file exists which may not
	 * really be the case.
	 */
	while (!we_created_flagfile) {
		int errsv, i, j;

		i = open_fd_mode(linkf, O_WRONLY|O_CREAT, 0);
		if (i < 0) {
			release_flag_file();
			return 0;
		}
		close(i);

		j = link(linkf, FLAG_FILE);
		errsv = errno;

		(void) unlink(linkf);

		if (j < 0 && errsv != EEXIST) {
			release_flag_file();
			return 0;
		}

		fd = open_fd(FLAG_FILE, O_RDWR);
		if (fd < 0) {
			/* Maybe the file was just deleted? */
			if (errno == ENOENT)
				continue;
			release_flag_file();
			return 0;
		}

		if (j == 0) {
			char pidbuf[MAX_PIDSIZE + 1];
			int pidlen;

			pidlen = sprintf(pidbuf, "%d\n", getpid());
			if (write(fd, pidbuf, pidlen) != pidlen) {
				release_flag_file();
				return 0;
			}

			we_created_flagfile = 1;
		} else {
			/*
			 * Someone else made the link.
			 * If the flag file is not owned by anyone clean
			 * it up and try again, otherwise return fail.
			 */
			if (!flag_is_owned(fd)) {
				close(fd);
				fd = -1;
				unlink(FLAG_FILE);
				continue;
			}

			release_flag_file();
			return 0;
		}

		close(fd);
		fd = -1;
	}

	return 1;
}