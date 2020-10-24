int openconnect_set_http_proxy(struct openconnect_info *vpninfo, char *proxy)
{
	char *url = proxy, *p;
	int ret;

	if (!url)
		return -ENOMEM;

	free(vpninfo->proxy_type);
	vpninfo->proxy_type = NULL;
	free(vpninfo->proxy);
	vpninfo->proxy = NULL;

	ret = internal_parse_url(url, &vpninfo->proxy_type, &vpninfo->proxy,
				 &vpninfo->proxy_port, NULL, 80);
	if (ret)
		goto out;

	p = strchr(vpninfo->proxy, '@');
	if (p) {
		/* Proxy username/password */
		*p = 0;
		vpninfo->proxy_user = vpninfo->proxy;
		vpninfo->proxy = strdup(p + 1);
		p = strchr(vpninfo->proxy_user, ':');
		if (p) {
			*p = 0;
			vpninfo->proxy_pass = strdup(p + 1);
		}
	}

	if (vpninfo->proxy_type &&
	    strcmp(vpninfo->proxy_type, "http") &&
	    strcmp(vpninfo->proxy_type, "socks") &&
	    strcmp(vpninfo->proxy_type, "socks5")) {
		vpn_progress(vpninfo, PRG_ERR,
			     _("Only http or socks(5) proxies supported\n"));
		free(vpninfo->proxy_type);
		vpninfo->proxy_type = NULL;
		free(vpninfo->proxy);
		vpninfo->proxy = NULL;
		return -EINVAL;
	}
 out:
	free(url);
	return ret;
}