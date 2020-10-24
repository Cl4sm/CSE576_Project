static int start_cstp_connection(struct openconnect_info *vpninfo)
{
	struct oc_text_buf *reqbuf;
	char buf[65536];
	int i;
	int dtls_secret_set = 0;
	int retried = 0, sessid_found = 0;
	struct oc_vpn_option **next_dtls_option = &vpninfo->dtls_options;
	struct oc_vpn_option **next_cstp_option = &vpninfo->cstp_options;
	struct oc_vpn_option *old_cstp_opts = vpninfo->cstp_options;
	struct oc_vpn_option *old_dtls_opts = vpninfo->dtls_options;
	const char *old_addr = vpninfo->ip_info.addr;
	const char *old_netmask = vpninfo->ip_info.netmask;
	const char *old_addr6 = vpninfo->ip_info.addr6;
	const char *old_netmask6 = vpninfo->ip_info.netmask6;
	int base_mtu, mtu;

	/* Clear old options which will be overwritten */
	vpninfo->ip_info.addr = vpninfo->ip_info.netmask = NULL;
	vpninfo->ip_info.addr6 = vpninfo->ip_info.netmask6 = NULL;
	vpninfo->cstp_options = vpninfo->dtls_options = NULL;
	vpninfo->ip_info.domain = vpninfo->ip_info.proxy_pac = NULL;
	vpninfo->banner = NULL;

	for (i = 0; i < 3; i++)
		vpninfo->ip_info.dns[i] = vpninfo->ip_info.nbns[i] = NULL;
	cstp_free_splits(vpninfo);

 retry:
	calculate_mtu(vpninfo, &base_mtu, &mtu);

	reqbuf = buf_alloc();
	buf_append(reqbuf, "CONNECT /CSCOSSLC/tunnel HTTP/1.1\r\n");
	buf_append(reqbuf, "Host: %s\r\n", vpninfo->hostname);
	buf_append(reqbuf, "User-Agent: %s\r\n", vpninfo->useragent);
	buf_append(reqbuf, "Cookie: webvpn=%s\r\n", vpninfo->cookie);
	buf_append(reqbuf, "X-CSTP-Version: 1\r\n");
	buf_append(reqbuf, "X-CSTP-Hostname: %s\r\n", vpninfo->localname);
	if (vpninfo->deflate && i < sizeof(buf))
		buf_append(reqbuf, "X-CSTP-Accept-Encoding: deflate;q=1.0\r\n");
	if (base_mtu)
		buf_append(reqbuf, "X-CSTP-Base-MTU: %d\r\n", base_mtu);
	buf_append(reqbuf, "X-CSTP-MTU: %d\r\n", mtu);
	buf_append(reqbuf, "X-CSTP-Address-Type: %s\r\n",
			       vpninfo->disable_ipv6 ? "IPv4" : "IPv6,IPv4");
	if (!vpninfo->disable_ipv6)
		buf_append(reqbuf, "X-CSTP-Full-IPv6-Capability: true\r\n");
	buf_append(reqbuf, "X-DTLS-Master-Secret: ");
	for (i = 0; i < sizeof(vpninfo->dtls_secret); i++) {
		buf_append(reqbuf, "%02X", vpninfo->dtls_secret[i]);
		dtls_secret_set |= vpninfo->dtls_secret[i];
	}
	if (!dtls_secret_set) {
		vpn_progress(vpninfo, PRG_ERR,
			     _("CRITICAL ERROR: DTLS master secret is uninitialised. Please report this.\n"));
		return -EINVAL;
	}

	buf_append(reqbuf, "\r\nX-DTLS-CipherSuite: %s\r\n\r\n",
			       vpninfo->dtls_ciphers ? : DEFAULT_CIPHER_LIST);

	if (buf_error(reqbuf)) {
		vpn_progress(vpninfo, PRG_ERR,
			     _("Error creating HTTPS CONNECT request\n"));
		return buf_free(reqbuf);
	}
	vpninfo->ssl_write(vpninfo, reqbuf->data, reqbuf->pos);
	buf_free(reqbuf);

	/* FIXME: Use process_http_response() instead of reimplementing it. It has
	   a header callback function, and can cope with CONNECT requests. */
	if ((i = vpninfo->ssl_gets(vpninfo, buf, 65536)) < 0) {
		if (i == -EINTR)
			return i;
		vpn_progress(vpninfo, PRG_ERR,
			     _("Error fetching HTTPS response\n"));
		if (!retried) {
			retried = 1;
			openconnect_close_https(vpninfo, 0);

			if (openconnect_open_https(vpninfo)) {
				vpn_progress(vpninfo, PRG_ERR,
					     _("Failed to open HTTPS connection to %s\n"),
					     vpninfo->hostname);
				return -EIO;
			}
			goto retry;
		}
		return -EINVAL;
	}

	if (strncmp(buf, "HTTP/1.1 200 ", 13)) {
		if (!strncmp(buf, "HTTP/1.1 503 ", 13)) {
			/* "Service Unavailable. Why? */
			const char *reason = "<unknown>";
			while ((i = vpninfo->ssl_gets(vpninfo, buf, sizeof(buf)))) {
				if (!strncmp(buf, "X-Reason: ", 10)) {
					reason = buf + 10;
					break;
				}
			}
			vpn_progress(vpninfo, PRG_ERR,
				     _("VPN service unavailable; reason: %s\n"),
				     reason);
			return -EINVAL;
		}
		vpn_progress(vpninfo, PRG_ERR,
			     _("Got inappropriate HTTP CONNECT response: %s\n"),
			     buf);
		if (!strncmp(buf, "HTTP/1.1 401 ", 13))
			return -EPERM;
		return -EINVAL;
	}

	vpn_progress(vpninfo, PRG_INFO, _("Got CONNECT response: %s\n"), buf);

	/* We may have advertised it, but we only do it if the server agrees */
	vpninfo->deflate = 0;
	mtu = 0;

	while ((i = vpninfo->ssl_gets(vpninfo, buf, sizeof(buf)))) {
		struct oc_vpn_option *new_option;
		char *colon;

		if (i < 0)
			return i;

		colon = strchr(buf, ':');
		if (!colon)
			continue;

		*colon = 0;
		colon++;
		if (*colon == ' ')
			colon++;

		if (strncmp(buf, "X-DTLS-", 7) &&
		    strncmp(buf, "X-CSTP-", 7))
			continue;

		new_option = malloc(sizeof(*new_option));
		if (!new_option) {
			vpn_progress(vpninfo, PRG_ERR, _("No memory for options\n"));
			return -ENOMEM;
		}
		new_option->option = strdup(buf);
		new_option->value = strdup(colon);
		new_option->next = NULL;

		if (!new_option->option || !new_option->value) {
			vpn_progress(vpninfo, PRG_ERR, _("No memory for options\n"));
			free(new_option->option);
			free(new_option->value);
			free(new_option);
			return -ENOMEM;
		}

		/* This contains the whole document, including the webvpn cookie. */
		if (!strcasecmp(buf, "X-CSTP-Post-Auth-XML"))
			vpn_progress(vpninfo, PRG_DEBUG, "%s: %s\n", buf, _("<elided>"));
		else
			vpn_progress(vpninfo, PRG_DEBUG, "%s: %s\n", buf, colon);

		if (!strncmp(buf, "X-DTLS-", 7)) {
			*next_dtls_option = new_option;
			next_dtls_option = &new_option->next;

			if (!strcmp(buf + 7, "MTU")) {
				int dtlsmtu = atol(colon);
				if (dtlsmtu > mtu)
					mtu = dtlsmtu;
			} else if (!strcmp(buf + 7, "Session-ID")) {
				if (strlen(colon) != 64) {
					vpn_progress(vpninfo, PRG_ERR,
						     _("X-DTLS-Session-ID not 64 characters; is: \"%s\"\n"),
						     colon);
					vpninfo->dtls_attempt_period = 0;
					return -EINVAL;
				}
				for (i = 0; i < 64; i += 2)
					vpninfo->dtls_session_id[i/2] = unhex(colon + i);
				sessid_found = 1;
			}
			continue;
		}
		/* CSTP options... */
		*next_cstp_option = new_option;
		next_cstp_option = &new_option->next;


		if (!strcmp(buf + 7, "Keepalive")) {
			vpninfo->ssl_times.keepalive = atol(colon);
		} else if (!strcmp(buf + 7, "DPD")) {
			int j = atol(colon);
			if (j && (!vpninfo->ssl_times.dpd || j < vpninfo->ssl_times.dpd))
				vpninfo->ssl_times.dpd = j;
		} else if (!strcmp(buf + 7, "Rekey-Time")) {
			vpninfo->ssl_times.rekey = atol(colon);
		} else if (!strcmp(buf + 7, "Rekey-Method")) {
			if (!strcmp(colon, "new-tunnel"))
				vpninfo->ssl_times.rekey_method = REKEY_TUNNEL;
			else if (!strcmp(colon, "ssl"))
				vpninfo->ssl_times.rekey_method = REKEY_SSL;
			else
				vpninfo->ssl_times.rekey_method = REKEY_NONE;
		} else if (!strcmp(buf + 7, "Content-Encoding")) {
			if (!strcmp(colon, "deflate"))
				vpninfo->deflate = 1;
			else {
				vpn_progress(vpninfo, PRG_ERR,
					     _("Unknown CSTP-Content-Encoding %s\n"),
					     colon);
				return -EINVAL;
			}
		} else if (!strcmp(buf + 7, "MTU")) {
			int cstpmtu = atol(colon);
			if (cstpmtu > mtu)
				mtu = cstpmtu;
		} else if (!strcmp(buf + 7, "Address-IP6")) {
			vpninfo->ip_info.netmask6 = new_option->value;
		} else if (!strcmp(buf + 7, "Address")) {
			if (strchr(new_option->value, ':')) {
				if (!vpninfo->disable_ipv6)
					vpninfo->ip_info.addr6 = new_option->value;
			} else
				vpninfo->ip_info.addr = new_option->value;
		} else if (!strcmp(buf + 7, "Netmask")) {
			if (strchr(new_option->value, ':')) {
				if (!vpninfo->disable_ipv6)
					vpninfo->ip_info.netmask6 = new_option->value;
			} else
				vpninfo->ip_info.netmask = new_option->value;
		} else if (!strcmp(buf + 7, "DNS")) {
			int j;
			for (j = 0; j < 3; j++) {
				if (!vpninfo->ip_info.dns[j]) {
					vpninfo->ip_info.dns[j] = new_option->value;
					break;
				}
			}
		} else if (!strcmp(buf + 7, "NBNS")) {
			int j;
			for (j = 0; j < 3; j++) {
				if (!vpninfo->ip_info.nbns[j]) {
					vpninfo->ip_info.nbns[j] = new_option->value;
					break;
				}
			}
		} else if (!strcmp(buf + 7, "Default-Domain")) {
			vpninfo->ip_info.domain = new_option->value;
		} else if (!strcmp(buf + 7, "MSIE-Proxy-PAC-URL")) {
			vpninfo->ip_info.proxy_pac = new_option->value;
		} else if (!strcmp(buf + 7, "Banner")) {
			vpninfo->banner = new_option->value;
		} else if (!strcmp(buf + 7, "Split-DNS")) {
			struct oc_split_include *dns = malloc(sizeof(*dns));
			if (!dns)
				continue;
			dns->route = new_option->value;
			dns->next = vpninfo->ip_info.split_dns;
			vpninfo->ip_info.split_dns = dns;
		} else if (!strcmp(buf + 7, "Split-Include") || !strcmp(buf + 7, "Split-Include-IP6")) {
			struct oc_split_include *inc = malloc(sizeof(*inc));
			if (!inc)
				continue;
			inc->route = new_option->value;
			inc->next = vpninfo->ip_info.split_includes;
			vpninfo->ip_info.split_includes = inc;
		} else if (!strcmp(buf + 7, "Split-Exclude") || !strcmp(buf + 7, "Split-Exclude-IP6")) {
			struct oc_split_include *exc = malloc(sizeof(*exc));
			if (!exc)
				continue;
			exc->route = new_option->value;
			exc->next = vpninfo->ip_info.split_excludes;
			vpninfo->ip_info.split_excludes = exc;
		}
	}

	if (!mtu) {
		vpn_progress(vpninfo, PRG_ERR,
			     _("No MTU received. Aborting\n"));
		return -EINVAL;
	}
	vpninfo->ip_info.mtu = mtu;

	if (!vpninfo->ip_info.addr && !vpninfo->ip_info.addr6) {
		vpn_progress(vpninfo, PRG_ERR,
			     _("No IP address received. Aborting\n"));
		return -EINVAL;
	}
	if (old_addr) {
		if (strcmp(old_addr, vpninfo->ip_info.addr)) {
			vpn_progress(vpninfo, PRG_ERR,
				     _("Reconnect gave different Legacy IP address (%s != %s)\n"),
				     vpninfo->ip_info.addr, old_addr);
			return -EINVAL;
		}
	}
	if (old_netmask) {
		if (strcmp(old_netmask, vpninfo->ip_info.netmask)) {
			vpn_progress(vpninfo, PRG_ERR,
				     _("Reconnect gave different Legacy IP netmask (%s != %s)\n"),
				     vpninfo->ip_info.netmask, old_netmask);
			return -EINVAL;
		}
	}
	if (old_addr6) {
		if (strcmp(old_addr6, vpninfo->ip_info.addr6)) {
			vpn_progress(vpninfo, PRG_ERR,
				     _("Reconnect gave different IPv6 address (%s != %s)\n"),
				     vpninfo->ip_info.addr6, old_addr6);
			return -EINVAL;
		}
	}
	if (old_netmask6) {
		if (strcmp(old_netmask6, vpninfo->ip_info.netmask6)) {
			vpn_progress(vpninfo, PRG_ERR,
				     _("Reconnect gave different IPv6 netmask (%s != %s)\n"),
				     vpninfo->ip_info.netmask6, old_netmask6);
			return -EINVAL;
		}
	}

	while (old_dtls_opts) {
		struct oc_vpn_option *tmp = old_dtls_opts;
		old_dtls_opts = old_dtls_opts->next;
		free(tmp->value);
		free(tmp->option);
		free(tmp);
	}
	while (old_cstp_opts) {
		struct oc_vpn_option *tmp = old_cstp_opts;
		old_cstp_opts = old_cstp_opts->next;
		free(tmp->value);
		free(tmp->option);
		free(tmp);
	}
	vpn_progress(vpninfo, PRG_INFO, _("CSTP connected. DPD %d, Keepalive %d\n"),
		     vpninfo->ssl_times.dpd, vpninfo->ssl_times.keepalive);


	monitor_fd_new(vpninfo, ssl);

	monitor_read_fd(vpninfo, ssl);
	monitor_except_fd(vpninfo, ssl);

	if (!sessid_found)
		vpninfo->dtls_attempt_period = 0;

	if (vpninfo->ssl_times.rekey <= 0)
		vpninfo->ssl_times.rekey_method = REKEY_NONE;

	vpninfo->ssl_times.last_rekey = vpninfo->ssl_times.last_rx =
		vpninfo->ssl_times.last_tx = time(NULL);
	return 0;
}