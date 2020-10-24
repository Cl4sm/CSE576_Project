static int handle_redirect(struct openconnect_info *vpninfo)
{
	vpninfo->redirect_type = REDIR_TYPE_LOCAL;

	if (!strncmp(vpninfo->redirect_url, "https://", 8)) {
		/* New host. Tear down the existing connection and make a new one */
		char *host;
		int port;
		int ret;

		free(vpninfo->urlpath);
		vpninfo->urlpath = NULL;

		ret = internal_parse_url(vpninfo->redirect_url, NULL, &host, &port, &vpninfo->urlpath, 0);
		if (ret) {
			vpn_progress(vpninfo, PRG_ERR,
				     _("Failed to parse redirected URL '%s': %s\n"),
				     vpninfo->redirect_url, strerror(-ret));
			free(vpninfo->redirect_url);
			vpninfo->redirect_url = NULL;
			return ret;
		}

		if (strcasecmp(vpninfo->hostname, host) || port != vpninfo->port) {
			openconnect_set_hostname(vpninfo, host);
			vpninfo->port = port;

			/* Kill the existing connection, and a new one will happen */
			openconnect_close_https(vpninfo, 0);
			clear_cookies(vpninfo);
			vpninfo->redirect_type = REDIR_TYPE_NEWHOST;
		} else
			free(host);

		free(vpninfo->redirect_url);
		vpninfo->redirect_url = NULL;

		return 0;
	} else if (strstr(vpninfo->redirect_url, "://")) {
		vpn_progress(vpninfo, PRG_ERR,
			     _("Cannot follow redirection to non-https URL '%s'\n"),
			     vpninfo->redirect_url);
		free(vpninfo->redirect_url);
		vpninfo->redirect_url = NULL;
		return -EINVAL;
	} else if (vpninfo->redirect_url[0] == '/') {
		/* Absolute redirect within same host */
		free(vpninfo->urlpath);
		vpninfo->urlpath = strdup(vpninfo->redirect_url + 1);
		free(vpninfo->redirect_url);
		vpninfo->redirect_url = NULL;
		return 0;
	} else {
		char *lastslash = NULL;
		if (vpninfo->urlpath)
			lastslash = strrchr(vpninfo->urlpath, '/');
		if (!lastslash) {
			free(vpninfo->urlpath);
			vpninfo->urlpath = vpninfo->redirect_url;
			vpninfo->redirect_url = NULL;
		} else {
			char *oldurl = vpninfo->urlpath;
			*lastslash = 0;
			vpninfo->urlpath = NULL;
			if (asprintf(&vpninfo->urlpath, "%s/%s",
				     oldurl, vpninfo->redirect_url) == -1) {
				int err = -errno;
				vpn_progress(vpninfo, PRG_ERR,
					     _("Allocating new path for relative redirect failed: %s\n"),
					     strerror(-err));
				return err;
			}
			free(oldurl);
			free(vpninfo->redirect_url);
			vpninfo->redirect_url = NULL;
		}
		return 0;
	}
}