int send_packet(int fd, const char *buf, size_t len)
{
	int ret = 0;
	sigset_t set, old;

	/* Block SIGPIPE */
	sigemptyset(&set);
	sigaddset(&set, SIGPIPE);
	pthread_sigmask(SIG_BLOCK, &set, &old);

	if (write_all(fd, &len, sizeof(len)) != sizeof(len))
		ret = -1;
	if (!ret && write_all(fd, buf, len) != len)
		ret = -1;

	/* And unblock it again */
	pthread_sigmask(SIG_SETMASK, &old, NULL);

	return ret;
}
