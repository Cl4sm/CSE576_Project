static int flag_is_owned(int fd)
{
	int pid = 0, tries = 3;

	while (tries--) {
		char pidbuf[MAX_PIDSIZE + 1];
		int got;

		lseek(fd, 0, SEEK_SET);
		got = read(fd, pidbuf, MAX_PIDSIZE);
		/*
		 * We add a terminator to the pid to verify write complete.
		 * If the write isn't finished in 300 milliseconds then it's
		 * probably a stale lock file.
		 */
		if (got > 0 && pidbuf[got - 1] == '\n') {
			sscanf(pidbuf, "%d", &pid);
			break;
		} else {
			struct timespec t = { 0, 100000000 };
			struct timespec r;

			while (nanosleep(&t, &r) == -1 && errno == EINTR)
				memcpy(&t, &r, sizeof(struct timespec));

			continue;
		}
	}

	/* Stale flagfile */
	if (!tries)
		return 0;

	if (pid) {
		int ret;

		ret = kill(pid, 0);
		/*
		 * If lock file exists but is not owned by a process
		 * we return unowned status so we can get rid of it
		 * and continue.
		 */
		if (ret == -1 && errno == ESRCH)
			return 0;
	} else {
		/*
		 * Odd, no pid in file - so what should we do?
		 * Assume something bad happened to owner and
		 * return unowned status.
		 */
		return 0;
	}

	return 1;
}