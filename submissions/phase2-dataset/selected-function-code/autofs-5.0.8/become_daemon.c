static void become_daemon(unsigned foreground, unsigned daemon_check)
{
	FILE *pidfp;
	char buf[MAX_ERR_BUF];
	int res;
	pid_t pid;

	/* Don't BUSY any directories unnecessarily */
	if (chdir("/")) {
		fprintf(stderr, "%s: failed change working directory.\n",
			program);
		exit(0);
	}

	if (open_pipe(start_pipefd) < 0) {
		fprintf(stderr, "%s: failed to create start_pipefd.\n",
			program);
		exit(0);
	}

	/* Detach from foreground process */
	if (foreground) {
		if (daemon_check && !aquire_flag_file()) {
			fprintf(stderr, "%s: program is already running.\n",
				program);
			exit(1);
		}
		log_to_stderr();
	} else {
		pid = fork();
		if (pid > 0) {
			close(start_pipefd[1]);
			res = read(start_pipefd[0], pst_stat, sizeof(*pst_stat));
			if (res < 0)
				exit(1);
			exit(*pst_stat);
		} else if (pid < 0) {
			fprintf(stderr, "%s: Could not detach process\n",
				program);
			exit(1);
		}
		close(start_pipefd[0]);

		if (daemon_check && !aquire_flag_file()) {
			fprintf(stderr, "%s: program is already running.\n",
				program);
			/* Return success if already running */
			st_stat = 0;
			res = write(start_pipefd[1], pst_stat, sizeof(*pst_stat));
			if (res < 0)
				exit(1);
			close(start_pipefd[1]);
			exit(*pst_stat);
		}

		/*
		 * Make our own process group for "magic" reason: processes that share
		 * our pgrp see the raw filesystem behind the magic.
		 */
		if (setsid() == -1) {
			char *estr = strerror_r(errno, buf, MAX_ERR_BUF);
			fprintf(stderr, "setsid: %s", estr);
			res = write(start_pipefd[1], pst_stat, sizeof(*pst_stat));
			close(start_pipefd[1]);
			exit(*pst_stat);
		}
		log_to_syslog();
	}

	/* Write pid file if requested */
	if (pid_file) {
		if ((pidfp = fopen(pid_file, "wt"))) {
			fprintf(pidfp, "%lu\n", (unsigned long) getpid());
			fclose(pidfp);
		} else {
			char *estr = strerror_r(errno, buf, MAX_ERR_BUF);
			logerr("failed to write pid file %s: %s",
			       pid_file, estr);
			pid_file = NULL;
		}
	}
}