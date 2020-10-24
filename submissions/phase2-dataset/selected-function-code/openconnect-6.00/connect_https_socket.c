int connect_https_socket(struct openconnect_info *vpninfo)
{
	int ssl_sock = -1;
	int err;

	if (!vpninfo->port)
		vpninfo->port = 443;

	if (vpninfo->peer_addr) {
	reconnect:
#ifdef SOCK_CLOEXEC
		ssl_sock = socket(vpninfo->peer_addr->sa_family, SOCK_STREAM | SOCK_CLOEXEC, IPPROTO_IP);
		if (ssl_sock < 0)
#endif
		{
			ssl_sock = socket(vpninfo->peer_addr->sa_family, SOCK_STREAM, IPPROTO_IP);
			if (ssl_sock < 0)
				goto reconn_err;
			set_fd_cloexec(ssl_sock);
		}
		if (cancellable_connect(vpninfo, ssl_sock, vpninfo->peer_addr, vpninfo->peer_addrlen)) {
		reconn_err:
			if (vpninfo->proxy) {
				vpn_progress(vpninfo, PRG_ERR,
					     _("Failed to reconnect to proxy %s\n"),
					     vpninfo->proxy);
			} else {
				vpn_progress(vpninfo, PRG_ERR,
					     _("Failed to reconnect to host %s\n"),
					     vpninfo->hostname);
			}
			if (ssl_sock >= 0)
				closesocket(ssl_sock);
			return -EINVAL;
		}
	} else {
		struct addrinfo hints, *result, *rp;
		char *hostname;
		char port[6];

		memset(&hints, 0, sizeof(struct addrinfo));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_PASSIVE | AI_NUMERICSERV;
		hints.ai_protocol = 0;
		hints.ai_canonname = NULL;
		hints.ai_addr = NULL;
		hints.ai_next = NULL;

		/* The 'port' variable is a string because it's easier
		   this way than if we pass NULL to getaddrinfo() and
		   then try to fill in the numeric value into
		   different types of returned sockaddr_in{6,}. */
#ifdef LIBPROXY_HDR
		if (vpninfo->proxy_factory) {
			char *url;
			char **proxies;
			int i = 0;

			free(vpninfo->proxy_type);
			vpninfo->proxy_type = NULL;
			free(vpninfo->proxy);
			vpninfo->proxy = NULL;

			if (vpninfo->port == 443)
				i = asprintf(&url, "https://%s/%s", vpninfo->hostname,
					     vpninfo->urlpath?:"");
			else
				i = asprintf(&url, "https://%s:%d/%s", vpninfo->hostname,
					     vpninfo->port, vpninfo->urlpath?:"");
			if (i == -1)
				return -ENOMEM;

			proxies = px_proxy_factory_get_proxies(vpninfo->proxy_factory,
							       url);

			i = 0;
			while (proxies && proxies[i]) {
				if (!vpninfo->proxy &&
				    (!strncmp(proxies[i], "http://", 7) ||
				     !strncmp(proxies[i], "socks://", 8) ||
				     !strncmp(proxies[i], "socks5://", 9)))
					internal_parse_url(proxies[i], &vpninfo->proxy_type,
						  &vpninfo->proxy, &vpninfo->proxy_port,
						  NULL, 0);
				i++;
			}
			free(url);
			free(proxies);
			if (vpninfo->proxy)
				vpn_progress(vpninfo, PRG_DEBUG,
					     _("Proxy from libproxy: %s://%s:%d/\n"),
					     vpninfo->proxy_type, vpninfo->proxy, vpninfo->port);
		}
#endif
		if (vpninfo->proxy) {
			hostname = vpninfo->proxy;
			snprintf(port, 6, "%d", vpninfo->proxy_port);
		} else {
			hostname = vpninfo->hostname;
			snprintf(port, 6, "%d", vpninfo->port);
		}

		if (hostname[0] == '[' && hostname[strlen(hostname)-1] == ']') {
			hostname = strndup(hostname + 1, strlen(hostname) - 2);
			if (!hostname)
				return -ENOMEM;
			hints.ai_flags |= AI_NUMERICHOST;
		}

		err = getaddrinfo(hostname, port, &hints, &result);

		if (err) {
			vpn_progress(vpninfo, PRG_ERR,
				     _("getaddrinfo failed for host '%s': %s\n"),
				     hostname, gai_strerror(err));
			if (hints.ai_flags & AI_NUMERICHOST)
				free(hostname);
			return -EINVAL;
		}
		if (hints.ai_flags & AI_NUMERICHOST)
			free(hostname);

		for (rp = result; rp ; rp = rp->ai_next) {
			char host[80];

			host[0] = 0;
			if (!getnameinfo(rp->ai_addr, rp->ai_addrlen, host,
					 sizeof(host), NULL, 0, NI_NUMERICHOST))
				vpn_progress(vpninfo, PRG_INFO, vpninfo->proxy_type ?
						     _("Attempting to connect to proxy %s%s%s:%s\n") :
						     _("Attempting to connect to server %s%s%s:%s\n"),
					     rp->ai_family == AF_INET6 ? "[" : "",
					     host,
					     rp->ai_family == AF_INET6 ? "]" : "",
					     port);

			ssl_sock = socket(rp->ai_family, rp->ai_socktype,
					  rp->ai_protocol);
			if (ssl_sock < 0)
				continue;
			set_fd_cloexec(ssl_sock);
			if (cancellable_connect(vpninfo, ssl_sock, rp->ai_addr, rp->ai_addrlen) >= 0) {
				/* Store the peer address we actually used, so that DTLS can
				   use it again later */
				vpninfo->peer_addr = malloc(rp->ai_addrlen);
				if (!vpninfo->peer_addr) {
					vpn_progress(vpninfo, PRG_ERR,
						     _("Failed to allocate sockaddr storage\n"));
					closesocket(ssl_sock);
					return -ENOMEM;
				}
				vpninfo->peer_addrlen = rp->ai_addrlen;
				memcpy(vpninfo->peer_addr, rp->ai_addr, rp->ai_addrlen);
				/* If no proxy, and if more than one address for the hostname,
				   ensure that we output the same IP address in authentication
				   results (from libopenconnect or --authenticate). */
				if (!vpninfo->proxy && (rp != result || rp->ai_next) && host[0]) {
					char *p = malloc(strlen(host) + 3);
					if (p) {
						free(vpninfo->unique_hostname);
						vpninfo->unique_hostname = p;
						if (rp->ai_family == AF_INET6)
							*p++ = '[';
						memcpy(p, host, strlen(host));
						p += strlen(host);
						if (rp->ai_family == AF_INET6)
							*p++ = ']';
						*p = 0;
					}
				}
				break;
			}
			closesocket(ssl_sock);
			ssl_sock = -1;
		}
		freeaddrinfo(result);

		if (ssl_sock < 0) {
			vpn_progress(vpninfo, PRG_ERR,
				     _("Failed to connect to host %s\n"),
				     vpninfo->proxy?:vpninfo->hostname);
			return -EINVAL;
		}
	}

	if (vpninfo->proxy) {
		err = process_proxy(vpninfo, ssl_sock);
		if (err) {
			closesocket(ssl_sock);
			if (err == -EAGAIN) {
				/* Proxy authentication failed and we need to retry */
				vpn_progress(vpninfo, PRG_DEBUG,
					     _("Reconnecting to proxy %s\n"), vpninfo->proxy);
				goto reconnect;
			}
			return err;
		}
	}

	return ssl_sock;
}