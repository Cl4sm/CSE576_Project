static int
daemonize(void)
{
	int pid;
	int dev_null_fd;

	if( (pid = fork()) < 0){
		fprintf(stderr, "Failed first fork : %s\n", strerror(errno));
		return -1;
	}
	else if (pid != 0)
		return pid;

	setsid();

	if ( (pid = fork()) < 0)
		fprintf(stderr, "Failed second fork : %s\n", strerror(errno));
	else if (pid != 0)
		_exit(0);

	if (chdir("/") < 0)
		fprintf(stderr, "cannot chdir to '/', continuing\n");

	dev_null_fd = open("/dev/null", O_RDWR);
	if (dev_null_fd < 0){
		fprintf(stderr, "cannot open /dev/null for input & output : %s\n",
			strerror(errno));
		_exit(0);
	}

	close(STDIN_FILENO);
	if (dup(dev_null_fd) < 0) {
		fprintf(stderr, "cannot dup /dev/null to stdin : %s\n",
			strerror(errno));
		_exit(0);
	}
	close(STDOUT_FILENO);
	if (dup(dev_null_fd) < 0) {
		fprintf(stderr, "cannot dup /dev/null to stdout : %s\n",
			strerror(errno));
		_exit(0);
	}
	close(STDERR_FILENO);
	if (dup(dev_null_fd) < 0) {
		fprintf(stderr, "cannot dup /dev/null to stderr : %s\n",
			strerror(errno));
		_exit(0);
	}
	close(dev_null_fd);
	daemon_pid = getpid();
	return 0;
}
