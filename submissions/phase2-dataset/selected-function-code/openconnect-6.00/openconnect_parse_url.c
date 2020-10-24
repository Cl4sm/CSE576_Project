int openconnect_parse_url(struct openconnect_info *vpninfo, char *url)
{
	char *scheme = NULL;
	int ret;

	openconnect_set_hostname(vpninfo, NULL);
	free(vpninfo->urlpath);
	vpninfo->urlpath = NULL;

	ret = internal_parse_url(url, &scheme, &vpninfo->hostname,
				 &vpninfo->port, &vpninfo->urlpath, 443);

	if (ret) {
		vpn_progress(vpninfo, PRG_ERR,
			     _("Failed to parse server URL '%s'\n"),
			     url);
		return ret;
	}
	if (scheme && strcmp(scheme, "https")) {
		vpn_progress(vpninfo, PRG_ERR,
			     _("Only https:// permitted for server URL\n"));
		ret = -EINVAL;
	}
	free(scheme);
	return ret;
}