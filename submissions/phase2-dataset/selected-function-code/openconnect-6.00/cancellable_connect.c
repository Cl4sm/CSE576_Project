static int cancellable_connect(struct openconnect_info *vpninfo, int sockfd,
			       const struct sockaddr *addr, socklen_t addrlen)
{
	struct sockaddr_storage peer;
	socklen_t peerlen = sizeof(peer);
	fd_set wr_set, rd_set;
	int maxfd = sockfd;

	set_sock_nonblock(sockfd);
	if (vpninfo->protect_socket)
		vpninfo->protect_socket(vpninfo->cbdata, sockfd);

	if (connect(sockfd, addr, addrlen) < 0 && !connect_pending())
		return -1;

	do {
		FD_ZERO(&wr_set);
		FD_ZERO(&rd_set);
		FD_SET(sockfd, &wr_set);
		cmd_fd_set(vpninfo, &rd_set, &maxfd);

		select(maxfd + 1, &rd_set, &wr_set, NULL, NULL);
		if (is_cancel_pending(vpninfo, &rd_set)) {
			vpn_progress(vpninfo, PRG_ERR, _("Socket connect cancelled\n"));
			errno = EINTR;
			return -1;
		}
	} while (!FD_ISSET(sockfd, &wr_set) && !vpninfo->got_pause_cmd);

	/* Check whether connect() succeeded or failed by using
	   getpeername(). See http://cr.yp.to/docs/connect.html */
	return getpeername(sockfd, (void *)&peer, &peerlen);
}