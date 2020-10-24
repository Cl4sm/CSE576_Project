static int safe_open(const char *filename, int flags)
{
	int fd = -1;
	uid_t userid = 0;
	gid_t groupid = 0;
	char *sudoenv = NULL;

/* Try opening with O_DIRECT */
#ifdef O_DIRECT
	fd = open(filename,
		flags
		|O_WRONLY
		|O_CREAT
		|O_TRUNC
		|(force_directio_write?O_DIRECT:0),
		0666);
#endif
/* If that failed (or we don't support O_DIRECT) try opening without */
	if (fd == -1) {
		fd = open(filename,
			flags
			|O_WRONLY
			|O_CREAT
			|O_TRUNC,
			0666);
	}

	if (fd == -1)
		return fd;

	/* If we're running via sudo, we want to write files owned by the
	 * original user rather than root.
	 *
	 * TODO: make this some sort of config option */

	sudoenv = getenv("SUDO_UID");
	if (sudoenv != NULL) {
		userid = strtol(sudoenv, NULL, 10);
	}
	sudoenv = getenv("SUDO_GID");
	if (sudoenv != NULL) {
		groupid = strtol(sudoenv, NULL, 10);
	}
	
	if (userid != 0 && fchown(fd, userid, groupid) == -1) {
		perror("fchown");
		return -1;
	}

	return fd;
}