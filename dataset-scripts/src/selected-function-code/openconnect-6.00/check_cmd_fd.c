void check_cmd_fd(struct openconnect_info *vpninfo, fd_set *fds)
{
	char cmd;

	if (vpninfo->cmd_fd == -1 || !FD_ISSET(vpninfo->cmd_fd, fds))
		return;
	if (vpninfo->cmd_fd_write == -1) {
		/* legacy openconnect_set_cancel_fd() users */
		vpninfo->got_cancel_cmd = 1;
		return;
	}

	if (read(vpninfo->cmd_fd, &cmd, 1) != 1)
		return;

	switch (cmd) {
	case OC_CMD_CANCEL:
	case OC_CMD_DETACH:
		vpninfo->got_cancel_cmd = 1;
		vpninfo->cancel_type = cmd;
		break;
	case OC_CMD_PAUSE:
		vpninfo->got_pause_cmd = 1;
		break;
	case OC_CMD_STATS:
		if (vpninfo->stats_handler)
			vpninfo->stats_handler(vpninfo->cbdata, &vpninfo->stats);
	}
}