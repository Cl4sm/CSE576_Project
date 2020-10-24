static int mydaemon(const char *rootdir)
{
	int i;
	pid_t pid;

	if((pid = fork()) < 0)
		return -1;
	else if (pid)
		exit(EXIT_SUCCESS); /* parent says bye bye */

	if(setsid() < 0) {
		log_error(LOG_ARGS, "Could not setsid()");
		return -1;
	}

	if(signal(SIGHUP, SIG_IGN) == SIG_ERR) {
		log_error(LOG_ARGS, "Could not signal(SIGHUP, SIG_IGN)");
		return -1;
	}

	if((pid = fork()) < 0)
		return -1;
	else if (pid)
		exit(EXIT_SUCCESS); /* parent says bye bye */

	if(chdir(rootdir) < 0)
		log_error(LOG_ARGS, "Could not chdir(%s)", rootdir);

	i = sysconf(_SC_OPEN_MAX);
	if(i < 0)
		i = 256;
	while(i >= 0)
		close(i--);

	open("/dev/null", O_RDONLY);
	open("/dev/null", O_RDWR);
	open("/dev/null", O_RDWR);
	
	return 0;
}