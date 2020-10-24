int process_proxy(struct openconnect_info *vpninfo, int ssl_sock)
{
	int ret, i;

	vpninfo->proxy_fd = ssl_sock;
	vpninfo->ssl_read = proxy_read;
	vpninfo->ssl_write = proxy_write;
	vpninfo->ssl_gets = proxy_gets;

	if (!vpninfo->proxy_type || !strcmp(vpninfo->proxy_type, "http"))
		ret = process_http_proxy(vpninfo);
	else if (!strcmp(vpninfo->proxy_type, "socks") ||
		 !strcmp(vpninfo->proxy_type, "socks5"))
		ret = process_socks_proxy(vpninfo);
	else {
		vpn_progress(vpninfo, PRG_ERR, _("Unknown proxy type '%s'\n"),
			     vpninfo->proxy_type);
		ret = -EIO;
	}

	vpninfo->proxy_fd = -1;
	for (i = 0; i < sizeof(auth_methods) / sizeof(auth_methods[0]); i++)
		clear_auth_state(vpninfo, &auth_methods[i], 1);
	return ret;
}