static int socks_password_auth(struct openconnect_info *vpninfo)
{
	int ul, pl, i;
	char buf[1024];

	if (!vpninfo->proxy_user || !vpninfo->proxy_pass) {
		vpn_progress(vpninfo, PRG_ERR,
			     _("SOCKS server requested username/password but we have none\n"));
		return -EIO;
	}
	ul = strlen(vpninfo->proxy_user);
	pl = strlen(vpninfo->proxy_pass);

	if (ul > 255 || pl > 255) {
		vpn_progress(vpninfo, PRG_ERR,
			     _("Username and password for SOCKS authentication must be < 255 bytes\n"));
		return -EINVAL;
	}

	buf[0] = 1;
	buf[1] = ul;
	memcpy(buf + 2, vpninfo->proxy_user, ul);
	buf[2 + ul] = pl;
	memcpy(buf + 3 + ul, vpninfo->proxy_pass, pl);

	i = proxy_write(vpninfo, buf, 3 + ul + pl);
	/* Don't leave passwords lying around if we can easily avoid it... */
	memset(buf, 0, sizeof(buf));
	if (i < 0) {
		vpn_progress(vpninfo, PRG_ERR,
			     _("Error writing auth request to SOCKS proxy: %s\n"),
			     strerror(-i));
		return i;
	}


	if ((i = proxy_read(vpninfo, buf, 2)) < 0) {
		vpn_progress(vpninfo, PRG_ERR,
			     _("Error reading auth response from SOCKS proxy: %s\n"),
			     strerror(-i));
		return i;
	}

	if (buf[0] != 1) {
		vpn_progress(vpninfo, PRG_ERR,
			     _("Unexpected auth response from SOCKS proxy: %02x %02x\n"),
			     buf[0], buf[1]);
		return -EIO;
	}
	if (buf[1] == 0) {
		vpn_progress(vpninfo, PRG_DEBUG,
			     _("Authenticated to SOCKS server using password\n"));
		return 0;
	} else {
		vpn_progress(vpninfo, PRG_ERR,
			     _("Password authentication to SOCKS server failed\n"));
		return -EIO;
	}
}