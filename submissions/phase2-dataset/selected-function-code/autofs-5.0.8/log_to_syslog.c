void log_to_syslog(void)
{
	char buf[MAX_ERR_BUF];
	int nullfd;

	open_log();

	/* Redirect all our file descriptors to /dev/null */
	nullfd = open("/dev/null", O_RDWR);
	if (nullfd < 0) {
		char *estr = strerror_r(errno, buf, MAX_ERR_BUF);
		fprintf(stderr, "cannot open /dev/null: %s", estr);
		exit(1);
	}

	if (dup2(nullfd, STDIN_FILENO) < 0 ||
	    dup2(nullfd, STDOUT_FILENO) < 0 ||
	    dup2(nullfd, STDERR_FILENO) < 0) {
		char *estr = strerror_r(errno, buf, MAX_ERR_BUF);
		fprintf(stderr,
			"redirecting file descriptors failed: %s", estr);
		exit(1);
	}

	if (nullfd > 2)
		close(nullfd);

	return;
}