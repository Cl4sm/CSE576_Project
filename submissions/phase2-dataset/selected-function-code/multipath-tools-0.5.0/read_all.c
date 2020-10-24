{
	size_t total = 0;
	ssize_t n;
	int ret;
	struct pollfd pfd;

	while (len) {
		pfd.fd = fd;
		pfd.events = POLLIN;
		ret = poll(&pfd, 1, 1000);
		if (!ret) {
			errno = ETIMEDOUT;
			return total;
		} else if (ret < 0) {
			if (errno == EINTR)
				continue;
			return total;
		} else if (!pfd.revents & POLLIN)
			continue;
		n = read(fd, buf, len);
		if (n < 0) {
			if ((errno == EINTR) || (errno == EAGAIN))
				continue;
			return total;
		}
		if (!n)
			return total;
		buf = n + (char *)buf;
		len -= n;
		total += n;
	}
	return total;
}
