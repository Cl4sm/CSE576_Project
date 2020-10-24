static int openconnect_gnutls_read(struct openconnect_info *vpninfo, char *buf, size_t len)
{
	int done;

	while ((done = gnutls_record_recv(vpninfo->https_sess, buf, len)) < 0) {
		if (done == GNUTLS_E_AGAIN) {
			/* Wait for something to happen on the socket, or on cmd_fd */
			fd_set wr_set, rd_set;
			int maxfd = vpninfo->ssl_fd;

			FD_ZERO(&wr_set);
			FD_ZERO(&rd_set);

			if (gnutls_record_get_direction(vpninfo->https_sess))
				FD_SET(vpninfo->ssl_fd, &wr_set);
			else
				FD_SET(vpninfo->ssl_fd, &rd_set);

			cmd_fd_set(vpninfo, &rd_set, &maxfd);
			select(maxfd + 1, &rd_set, &wr_set, NULL, NULL);
			if (is_cancel_pending(vpninfo, &rd_set)) {
				vpn_progress(vpninfo, PRG_ERR, _("SSL read cancelled\n"));
				return -EINTR;
			}
#ifdef GNUTLS_E_PREMATURE_TERMINATION
		} else if (done == GNUTLS_E_PREMATURE_TERMINATION) {
			/* We've seen this with HTTP 1.0 responses followed by abrupt
			   socket closure and no clean SSL shutdown.
			   https://bugs.launchpad.net/bugs/1225276 */
			vpn_progress(vpninfo, PRG_DEBUG, _("SSL socket closed uncleanly\n"));
			return 0;
#endif
		} else {
			vpn_progress(vpninfo, PRG_ERR, _("Failed to read from SSL socket: %s\n"),
				     gnutls_strerror(done));
			return -EIO;
		}

	}
	return done;
}