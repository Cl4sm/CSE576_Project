static int openconnect_openssl_write(struct openconnect_info *vpninfo, char *buf, size_t len)
{
	size_t orig_len = len;

	while (len) {
		int done = SSL_write(vpninfo->https_ssl, buf, len);

		if (done > 0)
			len -= done;
		else {
			int err = SSL_get_error(vpninfo->https_ssl, done);
			fd_set wr_set, rd_set;
			int maxfd = vpninfo->ssl_fd;

			FD_ZERO(&wr_set);
			FD_ZERO(&rd_set);

			if (err == SSL_ERROR_WANT_READ)
				FD_SET(vpninfo->ssl_fd, &rd_set);
			else if (err == SSL_ERROR_WANT_WRITE)
				FD_SET(vpninfo->ssl_fd, &wr_set);
			else {
				vpn_progress(vpninfo, PRG_ERR, _("Failed to write to SSL socket\n"));
				openconnect_report_ssl_errors(vpninfo);
				return -EIO;
			}
			cmd_fd_set(vpninfo, &rd_set, &maxfd);
			select(maxfd + 1, &rd_set, &wr_set, NULL, NULL);
			if (is_cancel_pending(vpninfo, &rd_set)) {
				vpn_progress(vpninfo, PRG_ERR, _("SSL write cancelled\n"));
				return -EINTR;
			}
		}
	}
	return orig_len;
}