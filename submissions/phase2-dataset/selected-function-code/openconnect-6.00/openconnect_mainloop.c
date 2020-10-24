int openconnect_mainloop(struct openconnect_info *vpninfo,
			 int reconnect_timeout,
			 int reconnect_interval)
{
	int ret = 0;

	vpninfo->reconnect_timeout = reconnect_timeout;
	vpninfo->reconnect_interval = reconnect_interval;

	if (vpninfo->cmd_fd != -1) {
		monitor_fd_new(vpninfo, cmd);
		monitor_read_fd(vpninfo, cmd);
	}

	while (!vpninfo->quit_reason) {
		int did_work = 0;
		int timeout = INT_MAX;
#ifdef _WIN32
		HANDLE events[4];
		int nr_events = 0;
#else
		struct timeval tv;
		fd_set rfds, wfds, efds;
#endif

#ifdef HAVE_DTLS
		if (vpninfo->dtls_state != DTLS_DISABLED) {
			ret = dtls_mainloop(vpninfo, &timeout);
			if (vpninfo->quit_reason)
				break;
			did_work += ret;
		}
#endif

		ret = cstp_mainloop(vpninfo, &timeout);
		if (vpninfo->quit_reason)
			break;
		did_work += ret;

		/* Tun must be last because it will set/clear its bit
		   in the select_rfds according to the queue length */
		did_work += tun_mainloop(vpninfo, &timeout);
		if (vpninfo->quit_reason)
			break;

		poll_cmd_fd(vpninfo, 0);
		if (vpninfo->got_cancel_cmd) {
			if (vpninfo->cancel_type == OC_CMD_CANCEL) {
				vpninfo->quit_reason = "Aborted by caller";
				ret = -EINTR;
			} else {
				ret = -ECONNABORTED;
			}
			break;
		}
		if (vpninfo->got_pause_cmd) {
			/* close all connections and wait for the user to call
			   openconnect_mainloop() again */
			openconnect_close_https(vpninfo, 0);
			if (vpninfo->dtls_state != DTLS_DISABLED) {
				dtls_close(vpninfo);
				vpninfo->dtls_state = DTLS_SLEEPING;
				vpninfo->new_dtls_started = 0;
			}

			vpninfo->got_pause_cmd = 0;
			vpn_progress(vpninfo, PRG_INFO, _("Caller paused the connection\n"));
			return 0;
		}

		if (did_work)
			continue;

		vpn_progress(vpninfo, PRG_TRACE,
			     _("No work to do; sleeping for %d ms...\n"), timeout);

#ifdef _WIN32
		if (vpninfo->dtls_monitored) {
			WSAEventSelect(vpninfo->dtls_fd, vpninfo->dtls_event, vpninfo->dtls_monitored);
			events[nr_events++] = vpninfo->dtls_event;
		}
		if (vpninfo->ssl_monitored) {
			WSAEventSelect(vpninfo->ssl_fd, vpninfo->ssl_event, vpninfo->ssl_monitored);
			events[nr_events++] = vpninfo->ssl_event;
		}
		if (vpninfo->cmd_monitored) {
			WSAEventSelect(vpninfo->cmd_fd, vpninfo->cmd_event, vpninfo->cmd_monitored);
			events[nr_events++] = vpninfo->cmd_event;
		}
		if (vpninfo->tun_monitored) {
			events[nr_events++] = vpninfo->tun_rd_overlap.hEvent;
		}
		if (WaitForMultipleObjects(nr_events, events, FALSE, timeout) == WAIT_FAILED) {
			vpn_progress(vpninfo, PRG_ERR,
				     _("WaitForMultipleObjects failed: %lx\n"),
				     GetLastError());
		}
#else
		memcpy(&rfds, &vpninfo->_select_rfds, sizeof(rfds));
		memcpy(&wfds, &vpninfo->_select_wfds, sizeof(wfds));
		memcpy(&efds, &vpninfo->_select_efds, sizeof(efds));

		tv.tv_sec = timeout / 1000;
		tv.tv_usec = (timeout % 1000) * 1000;

		select(vpninfo->_select_nfds, &rfds, &wfds, &efds, &tv);
#endif
	}

	if (vpninfo->quit_reason)
		cstp_bye(vpninfo, vpninfo->quit_reason);

	os_shutdown_tun(vpninfo);
	return ret < 0 ? ret : -EIO;
}