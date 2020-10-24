static int timed_read(int pipe, char *buf, size_t len, int time)
{
	struct pollfd pfd[1];
	int timeout = time;
	int ret;

	pfd[0].fd = pipe;
	pfd[0].events = POLLIN;

	if (time != -1) {
		if (time >= (INT_MAX - 1)/1000)
			timeout = INT_MAX - 1;
		else
			timeout = time * 1000;
	}

	ret = poll(pfd, 1, timeout);
	if (ret <= 0) {
		if (ret == 0)
			ret = -ETIMEDOUT;
		return ret;
	}

	while ((ret = read(pipe, buf, len)) == -1 && errno == EINTR);

	return ret;
}