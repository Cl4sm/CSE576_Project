static int proxy_write(struct openconnect_info *vpninfo, char *buf, size_t len)
{
	size_t count;
	int fd = vpninfo->proxy_fd;

	if (fd == -1)
		return -EINVAL;

	for (count = 0; count < len; ) {
		fd_set rd_set, wr_set;
		int maxfd = fd;
		int i;

		FD_ZERO(&wr_set);
		FD_ZERO(&rd_set);
		FD_SET(fd, &wr_set);
		cmd_fd_set(vpninfo, &rd_set, &maxfd);

		select(maxfd + 1, &rd_set, &wr_set, NULL, NULL);
		if (is_cancel_pending(vpninfo, &rd_set))
			return -EINTR;

		/* Not that this should ever be able to happen... */
		if (!FD_ISSET(fd, &wr_set))
			continue;

		i = send(fd, (void *)&buf[count], len - count, 0);
		if (i < 0)
			return -errno;

		count += i;
	}
	return count;
}