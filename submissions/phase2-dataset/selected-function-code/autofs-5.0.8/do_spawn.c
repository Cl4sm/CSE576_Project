static int do_spawn(unsigned logopt, unsigned int wait,
		    unsigned int options, const char *prog,
		    const char *const *argv)
{
	pid_t f;
	int ret, status, pipefd[2];
	char errbuf[ERRBUFSIZ + 1], *p, *sp;
	int errp, errn;
	int cancel_state;
	unsigned int use_lock = options & SPAWN_OPT_LOCK;
	unsigned int use_access = options & SPAWN_OPT_ACCESS;
	sigset_t allsigs, tmpsig, oldsig;
	struct thread_stdenv_vars *tsv;
	pid_t euid = 0;
	gid_t egid = 0;

	if (open_pipe(pipefd))
		return -1;

	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &cancel_state);

	sigfillset(&allsigs);
	pthread_sigmask(SIG_BLOCK, &allsigs, &oldsig);

	if (use_lock) {
		status = pthread_mutex_lock(&spawn_mutex);
		if (status)
			fatal(status);
	}

	tsv = pthread_getspecific(key_thread_stdenv_vars);
	if (tsv) {
		euid = tsv->uid;
		egid = tsv->gid;
	}

	f = fork();
	if (f == 0) {
		char **pargv = (char **) argv;
		int loc = 0;

		reset_signals();
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		dup2(pipefd[1], STDERR_FILENO);
		close(pipefd[1]);

		/* what to mount must always be second last */
		while (*pargv++)
			loc++;
		loc -= 2;

		/*
		 * If the mount location starts with a "/" then it is
		 * a local path. In this case it is a bind mount, a
		 * loopback mount or a file system that uses a local
		 * path so we need to check for dependent mounts.
		 *
		 * I hope host names are never allowed "/" as first char
		 */
		if (use_access && *(argv[loc]) == '/') {
			pid_t pgrp = getpgrp();

			/*
			 * Pretend to be requesting user and set non-autofs
			 * program group to trigger mount
			 */
			if (euid) {
				seteuid(euid);
				setegid(egid);
			}
			setpgrp();

			/*
			 * Trigger the recursive mount.
			 *
			 * Ignore the access(2) return code as there may be
			 * multiple waiters for this mount and we need to
			 * let the  VFS handle access returns to each
			 * individual waiter.
			 */
			access(argv[loc], F_OK);

			seteuid(0);
			setegid(0);
			if (pgrp >= 0)
				setpgid(0, pgrp);
		}

		execv(prog, (char *const *) argv);
		_exit(255);	/* execv() failed */
	} else {
		tmpsig = oldsig;

		sigaddset(&tmpsig, SIGCHLD);
		pthread_sigmask(SIG_SETMASK, &tmpsig, NULL);

		close(pipefd[1]);

		if (f < 0) {
			close(pipefd[0]);
			if (use_lock) {
				status = pthread_mutex_unlock(&spawn_mutex);
				if (status)
					fatal(status);
			}
			pthread_sigmask(SIG_SETMASK, &oldsig, NULL);
			pthread_setcancelstate(cancel_state, NULL);
			return -1;
		}

		errp = 0;
		do {
			errn = timed_read(pipefd[0],
					  errbuf + errp, ERRBUFSIZ - errp, wait);
			if (errn > 0) {
				errp += errn;

				sp = errbuf;
				while (errp && (p = memchr(sp, '\n', errp))) {
					*p++ = '\0';
					if (sp[0])	/* Don't output empty lines */
						warn(logopt, ">> %s", sp);
					errp -= (p - sp);
					sp = p;
				}

				if (errp && sp != errbuf)
					memmove(errbuf, sp, errp);

				if (errp >= ERRBUFSIZ) {
					/* Line too long, split */
					errbuf[errp] = '\0';
					warn(logopt, ">> %s", errbuf);
					errp = 0;
				}
			}
		} while (errn > 0);

		if (errn == -ETIMEDOUT)
			kill(f, SIGTERM);

		close(pipefd[0]);

		if (errp > 0) {
			/* End of file without \n */
			errbuf[errp] = '\0';
			warn(logopt, ">> %s", errbuf);
		}

		if (waitpid(f, &ret, 0) != f)
			ret = -1;	/* waitpid() failed */

		if (use_lock) {
			status = pthread_mutex_unlock(&spawn_mutex);
			if (status)
				fatal(status);
		}
		pthread_sigmask(SIG_SETMASK, &oldsig, NULL);
		pthread_setcancelstate(cancel_state, NULL);

		return ret;
	}
}