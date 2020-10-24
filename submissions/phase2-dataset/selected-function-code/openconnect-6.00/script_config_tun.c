int script_config_tun(struct openconnect_info *vpninfo, const char *reason)
{
	int ret;

	if (!vpninfo->vpnc_script || vpninfo->script_tun)
		return 0;

	setenv("reason", reason, 1);
	ret = system(vpninfo->vpnc_script);
	if (ret == -1) {
		int e = errno;
		vpn_progress(vpninfo, PRG_ERR,
			     _("Failed to spawn script '%s' for %s: %s\n"),
			     vpninfo->vpnc_script, reason, strerror(e));
		return -e;
	}
#ifdef _WIN32
	if (ret == 0x2331) {
		/* This is what cmd.exe returns for unrecognised commands */
		vpn_progress(vpninfo, PRG_ERR,
			     _("Failed to spawn script '%s' for %s: %s\n"),
			     vpninfo->vpnc_script, reason, strerror(ENOENT));
		return -ENOENT;
	}
#else
	if (!WIFEXITED(ret)) {
		vpn_progress(vpninfo, PRG_ERR,
			     _("Script '%s' exited abnormally (%x)\n"),
			       vpninfo->vpnc_script, ret);
		return -EIO;
	}

	ret = WEXITSTATUS(ret);
#endif
	if (ret) {
		vpn_progress(vpninfo, PRG_ERR,
			     _("Script '%s' returned error %d\n"),
			     vpninfo->vpnc_script, ret);
		return -EIO;
	}
	return 0;
}