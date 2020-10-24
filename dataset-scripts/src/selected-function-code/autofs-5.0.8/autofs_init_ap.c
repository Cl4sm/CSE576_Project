static int autofs_init_ap(struct autofs_point *ap)
{
	int pipefd[2];

	if ((ap->state != ST_INIT)) {
		/* This can happen if an autofs process is already running*/
		error(ap->logopt, "bad state %d", ap->state);
		return -1;
	}

	ap->pipefd = ap->kpipefd = ap->ioctlfd = -1;

	/* Pipe for kernel communications */
	if (open_pipe(pipefd) < 0) {
		crit(ap->logopt,
		     "failed to create commumication pipe for autofs path %s",
		     ap->path);
		return -1;
	}

	ap->pipefd = pipefd[0];
	ap->kpipefd = pipefd[1];

	/* Pipe state changes from signal handler to main loop */
	if (open_pipe(ap->state_pipe) < 0) {
		crit(ap->logopt,
		     "failed create state pipe for autofs path %s", ap->path);
		close(ap->pipefd);
		close(ap->kpipefd);	/* Close kernel pipe end */
		return -1;
	}

	if (create_logpri_fifo(ap) < 0) {
		logmsg("could not create FIFO for path %s\n", ap->path);
		logmsg("dynamic log level changes not available for %s", ap->path);
	}

	return 0;
}