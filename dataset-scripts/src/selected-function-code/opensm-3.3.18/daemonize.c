static int daemonize(osm_opensm_t * osm)
{
	pid_t pid;
	int fd;
	FILE *f;

	fd = open("/dev/null", O_WRONLY);
	if (fd < 0) {
		perror("open");
		return -1;
	}

	if ((pid = fork()) < 0) {
		perror("fork");
		exit(-1);
	} else if (pid > 0)
		exit(0);

	setsid();

	if ((pid = fork()) < 0) {
		perror("fork");
		exit(-1);
	} else if (pid > 0)
		exit(0);

	if (pidfile) {
		remove_pidfile();
		f = fopen(pidfile, "w");
		if (f) {
			fprintf(f, "%d\n", getpid());
			fclose(f);
		} else {
			perror("fopen");
			exit(1);
		}
	}

	close(0);
	close(1);
	close(2);

	dup2(fd, 0);
	dup2(fd, 1);
	dup2(fd, 2);

	return 0;
}