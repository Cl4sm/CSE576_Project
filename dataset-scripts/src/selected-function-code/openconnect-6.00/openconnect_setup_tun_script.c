int openconnect_setup_tun_script(struct openconnect_info *vpninfo, char *tun_script)
{
	pid_t child;
	int fds[2];

	vpninfo->vpnc_script = tun_script;
	vpninfo->script_tun = 1;

	set_script_env(vpninfo);
	if (socketpair(AF_UNIX, SOCK_DGRAM, 0, fds)) {
		vpn_progress(vpninfo, PRG_ERR, _("socketpair failed: %s\n"), strerror(errno));
		return -EIO;
	}
	child = fork();
	if (child < 0) {
		vpn_progress(vpninfo, PRG_ERR, _("fork failed: %s\n"), strerror(errno));
		return -EIO;
	} else if (!child) {
		if (setpgid(0, getpid()) < 0)
			perror(_("setpgid"));
		close(fds[0]);
		setenv_int("VPNFD", fds[1]);
		execl("/bin/sh", "/bin/sh", "-c", vpninfo->vpnc_script, NULL);
		perror(_("execl"));
		exit(1);
	}
	close(fds[1]);
	vpninfo->script_tun = child;
	vpninfo->ifname = strdup(_("(script)"));

	return openconnect_setup_tun_fd(vpninfo, fds[0]);
}