static int connect_nb(int fd, struct sockaddr *addr, socklen_t len, struct timeval *tout)
{
	struct pollfd pfd[1];
	int timeout = tout->tv_sec;
	int flags, ret;

	flags = fcntl(fd, F_GETFL, 0);
	if (flags < 0)
		return -errno;

	ret = fcntl(fd, F_SETFL, flags | O_NONBLOCK);
	if (ret < 0)
		return -errno;

	/* 
	 * From here on subsequent sys calls could change errno so
	 * we set ret = -errno to capture it in case we decide to
	 * use it later.
	 */
	ret = connect(fd, addr, len);
	if (ret < 0 && errno != EINPROGRESS) {
		ret = -errno;
		goto done;
	}

	if (ret == 0)
		goto done;

	if (timeout != -1) {
		if (timeout >= (INT_MAX - 1)/1000)
			timeout = INT_MAX - 1;
		else
			timeout = timeout * 1000;
	}

	pfd[0].fd = fd;
	pfd[0].events = POLLOUT;

	ret = poll(pfd, 1, timeout);
	if (ret <= 0) {
		if (ret == 0)
			ret = -ETIMEDOUT;
		else
			ret = -errno;
		goto done;
	}

	if (pfd[0].revents) {
		int status;

		len = sizeof(ret);
		status = getsockopt(fd, SOL_SOCKET, SO_ERROR, &ret, &len);
		if (status < 0) {
			char buf[MAX_ERR_BUF + 1];
			char *estr = strerror_r(errno, buf, MAX_ERR_BUF);

			/*
			 * We assume getsockopt amounts to a read on the
			 * descriptor and gives us the errno we need for
			 * the POLLERR revent case.
			 */
			ret = -errno;

			/* Unexpected case, log it so we know we got caught */
			if (pfd[0].revents & POLLNVAL)
				logerr("unexpected poll(2) error on connect:"
				       " %s", estr);

			goto done;
		}

		/* Oops - something wrong with connect */
		if (ret)
			ret = -ret;
	}

done:
	fcntl(fd, F_SETFL, flags);
	return ret;
}