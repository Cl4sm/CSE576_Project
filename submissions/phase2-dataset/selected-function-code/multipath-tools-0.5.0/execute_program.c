int execute_program(char *path, char *value, int len)
{
	int retval;
	int count;
	int status;
	int fds[2], null_fd;
	pid_t pid;
	char *pos;
	char arg[CALLOUT_MAX_SIZE];
	int argc = sizeof(arg) / 2;
	char *argv[argc + 1];
	int i;

	i = 0;

	if (strchr(path, ' ')) {
		strlcpy(arg, path, sizeof(arg));
		pos = arg;
		while (pos != NULL && i < argc) {
			if (pos[0] == '\'') {
				/* don't separate if in apostrophes */
				pos++;
				argv[i] = strsep(&pos, "\'");
				while (pos[0] == ' ')
					pos++;
			} else {
				argv[i] = strsep(&pos, " ");
			}
			i++;
		}
	} else {
		argv[i++] = path;
	}
	argv[i] =  NULL;

	retval = pipe(fds);

	if (retval != 0) {
		condlog(0, "error creating pipe for callout: %s", strerror(errno));
		return -1;
	}

	pid = fork();

	switch(pid) {
	case 0:
		/* child */
		close(STDOUT_FILENO);

		/* dup write side of pipe to STDOUT */
		if (dup(fds[1]) < 0)
			return -1;

		/* Ignore writes to stderr */
		null_fd = open("/dev/null", O_WRONLY);
		if (null_fd > 0) {
			close(STDERR_FILENO);
			retval = dup(null_fd);
			close(null_fd);
		}

		retval = execv(argv[0], argv);
		condlog(0, "error execing %s : %s", argv[0], strerror(errno));
		exit(-1);
	case -1:
		condlog(0, "fork failed: %s", strerror(errno));
		close(fds[0]);
		close(fds[1]);
		return -1;
	default:
		/* parent reads from fds[0] */
		close(fds[1]);
		retval = 0;
		i = 0;
		while (1) {
			count = read(fds[0], value + i, len - i-1);
			if (count <= 0)
				break;

			i += count;
			if (i >= len-1) {
				condlog(0, "not enough space for response from %s", argv[0]);
				retval = -1;
				break;
			}
		}

		if (count < 0) {
			condlog(0, "no response from %s", argv[0]);
			retval = -1;
		}

		if (i > 0 && value[i-1] == '\n')
			i--;
		value[i] = '\0';

		wait(&status);
		close(fds[0]);

		retval = -1;
		if (WIFEXITED(status)) {
			status = WEXITSTATUS(status);
			if (status == 0)
				retval = 0;
			else
				condlog(0, "%s exitted with %d", argv[0], status);
		}
		else if (WIFSIGNALED(status))
			condlog(0, "%s was terminated by signal %d", argv[0], WTERMSIG(status));
		else
			condlog(0, "%s terminated abnormally", argv[0]);
	}
	return retval;
}
