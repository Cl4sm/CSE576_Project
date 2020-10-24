RETSIGTYPE
reaper(int signo)
{
	register pid_t pid;
	DECLWAITSTATUS status;

	for (;;) {
		pid = waitpid((pid_t)0, &status, WNOHANG);
		if ((int)pid < 0) {
			/* ptrace foo */
			if (errno == EINTR)
				continue;
			/* ECHILD means no one left */
			if (errno != ECHILD)
				syslog(LOG_ERR, "reaper: %m");
			break;
		}
		/* Already got everyone who was done */
		if (pid == 0)
			break;
		--cdepth;
		if (WEXITSTATUS(status))
			syslog(LOG_DEBUG, "reaper: pid %d, exit status %d",
			    pid, WEXITSTATUS(status));
	}
	return RETSIGVAL;
}