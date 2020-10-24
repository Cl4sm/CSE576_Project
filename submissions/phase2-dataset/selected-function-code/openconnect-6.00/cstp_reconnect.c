static int cstp_reconnect(struct openconnect_info *vpninfo)
{
	int ret;
	int timeout;
	int interval;

	openconnect_close_https(vpninfo, 0);

	if (vpninfo->deflate) {
		/* Requeue the original packet that was deflated */
		if (vpninfo->current_ssl_pkt == vpninfo->deflate_pkt) {
			vpninfo->current_ssl_pkt = NULL;
			queue_packet(&vpninfo->outgoing_queue, vpninfo->pending_deflated_pkt);
			vpninfo->pending_deflated_pkt = NULL;
		}
		inflateEnd(&vpninfo->inflate_strm);
		deflateEnd(&vpninfo->deflate_strm);
	}
	timeout = vpninfo->reconnect_timeout;
	interval = vpninfo->reconnect_interval;

	while ((ret = openconnect_make_cstp_connection(vpninfo))) {
		if (timeout <= 0)
			return ret;
		if (ret == -EPERM) {
			vpn_progress(vpninfo, PRG_ERR,
				     _("Cookie is no longer valid, ending session\n"));
			return ret;
		}
		vpn_progress(vpninfo, PRG_INFO,
			     _("sleep %ds, remaining timeout %ds\n"),
			     interval, timeout);
		poll_cmd_fd(vpninfo, interval);
		if (vpninfo->got_cancel_cmd)
			return -EINTR;
		if (vpninfo->got_pause_cmd)
			return 0;
		timeout -= interval;
		interval += vpninfo->reconnect_interval;
		if (interval > RECONNECT_INTERVAL_MAX)
			interval = RECONNECT_INTERVAL_MAX;
	}
	script_config_tun(vpninfo, "reconnect");
	return 0;
}