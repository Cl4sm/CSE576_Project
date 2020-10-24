static int proxy_read(struct openconnect_info *vpninfo, char *buf, size_t len)
{
	size_t count;
	int fd = vpninfo->proxy_fd;

	if (fd == -1)
		return -EINVAL;

	for (count = 0; count < len; ) {
		fd_set rd_set;
		int maxfd = fd;
		int i;

		FD_ZERO(&rd_set);
		FD_SET(fd, &rd_set);
		cmd_fd_set(vpninfo, &rd_set, &maxfd);

		select(maxfd + 1, &rd_set, NULL, NULL, NULL);
		if (is_cancel_pending(vpninfo, &rd_set))
			return -EINTR;

		/* Not that this should ever be able to happen... */
		if (!FD_ISSET(fd, &rd_set))
			continue;

		i = recv(fd, (void *)&buf[count], len - count, 0);
		if (i < 0)
			return -errno;
		else if (i == 0)
			return -ECONNRESET;

		count += i;
	}
	return count;
}