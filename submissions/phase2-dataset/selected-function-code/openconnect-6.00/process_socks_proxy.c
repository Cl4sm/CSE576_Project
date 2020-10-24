static int process_socks_proxy(struct openconnect_info *vpninfo)
{
	char buf[1024];
	int i, nr_auth_methods = 0;

	buf[0] = 5; /* SOCKS version */

	buf[2 + nr_auth_methods++] = SOCKS_AUTH_NONE;
#if defined(HAVE_GSSAPI) || defined(_WIN32)
	if (vpninfo->auth[AUTH_TYPE_GSSAPI].state != AUTH_DISABLED &&
	    !vpninfo->proxy_user && !vpninfo->proxy_pass)
		buf[2 + nr_auth_methods++] = SOCKS_AUTH_GSSAPI;
#endif
	if (vpninfo->auth[AUTH_TYPE_BASIC].state != AUTH_DISABLED &&
	    vpninfo->proxy_user && vpninfo->proxy_pass)
		buf[2 + nr_auth_methods++] = SOCKS_AUTH_PASSWORD;

	buf[1] = nr_auth_methods;

	if ((i = proxy_write(vpninfo, buf, 2 + nr_auth_methods)) < 0) {
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
	if (buf[0] != 5) {
		vpn_progress(vpninfo, PRG_ERR,
			     _("Unexpected auth response from SOCKS proxy: %02x %02x\n"),
			     buf[0], buf[1]);
		return -EIO;
	}
	switch ((unsigned char)buf[1]) {
	case SOCKS_AUTH_NONE:
		/* No authentication */
		break;

	case SOCKS_AUTH_GSSAPI:
#if defined(HAVE_GSSAPI) || defined(_WIN32)
		vpn_progress(vpninfo, PRG_DEBUG,
			     _("SOCKS server requested GSSAPI authentication\n"));
		if (socks_gssapi_auth(vpninfo))
			return -EIO;
		break;
#else
		/* This should never happen since we didn't ask for it! */
		vpn_progress(vpninfo, PRG_ERR,
			     _("SOCKS server requested GSSAPI authentication\n"));
		return -EIO;
#endif

	case SOCKS_AUTH_PASSWORD:
		vpn_progress(vpninfo, PRG_DEBUG,
			     _("SOCKS server requested password authentication\n"));
		if (socks_password_auth(vpninfo))
			return -EIO;
		break;

	case SOCKS_AUTH_NO_ACCEPTABLE:
		vpn_progress(vpninfo, PRG_ERR,
			     _("SOCKS server requires authentication\n"));
		return -EIO;

	default:
		vpn_progress(vpninfo, PRG_ERR,
			     _("SOCKS server requested unknown authentication type %02x\n"),
			     (unsigned char)buf[1]);
		return -EIO;
	}

	vpn_progress(vpninfo, PRG_INFO,
		     _("Requesting SOCKS proxy connection to %s:%d\n"),
		     vpninfo->hostname, vpninfo->port);

	buf[0] = 5; /* SOCKS version */
	buf[1] = 1; /* CONNECT */
	buf[2] = 0; /* Reserved */
	buf[3] = 3; /* Address type is domain name */
	buf[4] = strlen(vpninfo->hostname);
	strcpy((char *)buf + 5, vpninfo->hostname);
	i = strlen(vpninfo->hostname) + 5;
	buf[i++] = vpninfo->port >> 8;
	buf[i++] = vpninfo->port & 0xff;

	if ((i = proxy_write(vpninfo, buf, i)) < 0) {
		vpn_progress(vpninfo, PRG_ERR,
			     _("Error writing connect request to SOCKS proxy: %s\n"),
			     strerror(-i));
		return i;
	}
	/* Read 5 bytes -- up to and including the first byte of the returned
	   address (which might be the length byte of a domain name) */
	if ((i = proxy_read(vpninfo, buf, 5)) < 0) {
		vpn_progress(vpninfo, PRG_ERR,
			     _("Error reading connect response from SOCKS proxy: %s\n"),
			     strerror(-i));
		return i;
	}
	if (i != 5 || buf[0] != 5) {
		vpn_progress(vpninfo, PRG_ERR,
			     _("Unexpected connect response from SOCKS proxy: %02x %02x...\n"),
			     buf[0], buf[1]);
		return -EIO;
	}
	if (buf[1]) {
		unsigned char err = buf[1];
		if (err < sizeof(socks_errors) / sizeof(socks_errors[0]))
			vpn_progress(vpninfo, PRG_ERR,
				     _("SOCKS proxy error %02x: %s\n"),
				     err, _(socks_errors[err]));
		else
			vpn_progress(vpninfo, PRG_ERR,
				     _("SOCKS proxy error %02x\n"), err);
		return -EIO;
	}

	/* Connect responses contain an address */
	switch (buf[3]) {
	case 1: /* Legacy IP */
		i = 5;
		break;
	case 3: /* Domain name */
		i = buf[4] + 2;
		break;
	case 4: /* IPv6 */
		i = 17;
		break;
	default:
		vpn_progress(vpninfo, PRG_ERR,
			     _("Unexpected address type %02x in SOCKS connect response\n"),
			     buf[3]);
		return -EIO;
	}

	if ((i = proxy_read(vpninfo, buf, i)) < 0) {
		vpn_progress(vpninfo, PRG_ERR,
			     _("Error reading connect response from SOCKS proxy: %s\n"),
			     strerror(-i));
		return i;
	}
	return 0;
}