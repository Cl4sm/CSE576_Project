static int
lock_file(int fd, char *file_name)
{
	struct sigaction act, oldact;
	sigset_t set, oldset;
	struct flock lock;
	int err;

	memset(&lock, 0, sizeof(lock));
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;

	act.sa_handler = sigalrm;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigemptyset(&set);
	sigaddset(&set, SIGALRM);

	sigaction(SIGALRM, &act, &oldact);
	pthread_sigmask(SIG_UNBLOCK, &set, &oldset);

	alarm(FILE_TIMEOUT);
	err = fcntl(fd, F_SETLKW, &lock);
	alarm(0);

	if (err) {
		if (errno != EINTR)
			condlog(0, "Cannot lock %s : %s", file_name,
				strerror(errno));
		else
			condlog(0, "%s is locked. Giving up.", file_name);
	}

	pthread_sigmask(SIG_SETMASK, &oldset, NULL);
	sigaction(SIGALRM, &oldact, NULL);
	return err;
}
