static int process_http_response(struct openconnect_info *vpninfo, int *result, int connect,
				 int (*header_cb)(struct openconnect_info *, char *, char *),
				 char **body_ret)
{
	char buf[MAX_BUF_LEN];
	char *body = NULL;
	int bodylen = BODY_HTTP10;
	int done = 0;
	int closeconn = 0;
	int i;

 cont:
	if (vpninfo->ssl_gets(vpninfo, buf, sizeof(buf)) < 0) {
		vpn_progress(vpninfo, PRG_ERR,
			     _("Error fetching HTTPS response\n"));
		openconnect_close_https(vpninfo, 0);
		return -EINVAL;
	}

	if (!strncmp(buf, "HTTP/1.0 ", 9))
		closeconn = 1;

	if ((!closeconn && strncmp(buf, "HTTP/1.1 ", 9)) || !(*result = atoi(buf+9))) {
		vpn_progress(vpninfo, PRG_ERR,
			     _("Failed to parse HTTP response '%s'\n"), buf);
		openconnect_close_https(vpninfo, 0);
		return -EINVAL;
	}

	vpn_progress(vpninfo, (*result == 200 || *result == 407) ? PRG_DEBUG : PRG_INFO,
		     _("Got HTTP response: %s\n"), buf);

	/* Eat headers... */
	while ((i = vpninfo->ssl_gets(vpninfo, buf, sizeof(buf)))) {
		char *colon;

		if (i < 0) {
			vpn_progress(vpninfo, PRG_ERR,
				     _("Error processing HTTP response\n"));
			openconnect_close_https(vpninfo, 0);
			return -EINVAL;
		}
		colon = strchr(buf, ':');
		if (!colon) {
			vpn_progress(vpninfo, PRG_ERR,
				     _("Ignoring unknown HTTP response line '%s'\n"), buf);
			continue;
		}
		*(colon++) = 0;
		if (*colon == ' ')
			colon++;

		/* Handle Set-Cookie first so that we can avoid printing the
		   webvpn cookie in the verbose debug output */
		if (!strcasecmp(buf, "Set-Cookie")) {
			char *semicolon = strchr(colon, ';');
			const char *print_equals;
			char *equals = strchr(colon, '=');
			int ret;

			if (semicolon)
				*semicolon = 0;

			if (!equals) {
				vpn_progress(vpninfo, PRG_ERR,
					     _("Invalid cookie offered: %s\n"), buf);
				return -EINVAL;
			}
			*(equals++) = 0;

			print_equals = equals;
			/* Don't print the webvpn cookie unless it's empty; we don't
			   want people posting it in public with debugging output */
			if (!strcmp(colon, "webvpn") && *equals)
				print_equals = _("<elided>");
			vpn_progress(vpninfo, PRG_DEBUG, "%s: %s=%s%s%s\n",
				     buf, colon, print_equals, semicolon ? ";" : "",
				     semicolon ? (semicolon+1) : "");

			/* The server tends to ask for the username and password as
			   usual, even if we've already failed because it didn't like
			   our cert. Thankfully it does give us this hint... */
			if (!strcmp(colon, "ClientCertAuthFailed"))
				vpn_progress(vpninfo, PRG_ERR,
					     _("SSL certificate authentication failed\n"));

			ret = http_add_cookie(vpninfo, colon, equals);
			if (ret)
				return ret;
		} else {
			vpn_progress(vpninfo, PRG_DEBUG, "%s: %s\n", buf, colon);
		}

		if (!strcasecmp(buf, "Connection")) {
			if (!strcasecmp(colon, "Close"))
				closeconn = 1;
#if 0
			/* This might seem reasonable, but in fact it breaks
			   certificate authentication with some servers. If
			   they give an HTTP/1.0 response, even if they
			   explicitly give a Connection: Keep-Alive header,
			   just close the connection. */
			else if (!strcasecmp(colon, "Keep-Alive"))
				closeconn = 0;
#endif
		}
		if (!strcasecmp(buf, "Location")) {
			vpninfo->redirect_url = strdup(colon);
			if (!vpninfo->redirect_url)
				return -ENOMEM;
		}
		if (!strcasecmp(buf, "Content-Length")) {
			bodylen = atoi(colon);
			if (bodylen < 0) {
				vpn_progress(vpninfo, PRG_ERR,
					     _("Response body has negative size (%d)\n"),
					     bodylen);
				openconnect_close_https(vpninfo, 0);
				return -EINVAL;
			}
		}
		if (!strcasecmp(buf, "Transfer-Encoding")) {
			if (!strcasecmp(colon, "chunked"))
				bodylen = BODY_CHUNKED;
			else {
				vpn_progress(vpninfo, PRG_ERR,
					     _("Unknown Transfer-Encoding: %s\n"),
					     colon);
				openconnect_close_https(vpninfo, 0);
				return -EINVAL;
			}
		}
		if (header_cb)
			header_cb(vpninfo, buf, colon);
	}

	/* Handle 'HTTP/1.1 100 Continue'. Not that we should ever see it */
	if (*result == 100)
		goto cont;

	/* On successful CONNECT, there is no body. Return success */
	if (connect && *result == 200)
		return 0;

	/* Now the body, if there is one */
	vpn_progress(vpninfo, PRG_DEBUG, _("HTTP body %s (%d)\n"),
		     bodylen == BODY_HTTP10 ? "http 1.0" :
		     bodylen == BODY_CHUNKED ? "chunked" : "length: ",
		     bodylen);

	/* If we were given Content-Length, it's nice and easy... */
	if (bodylen > 0) {
		body = malloc(bodylen + 1);
		if (!body)
			return -ENOMEM;
		while (done < bodylen) {
			i = vpninfo->ssl_read(vpninfo, body + done, bodylen - done);
			if (i < 0) {
				vpn_progress(vpninfo, PRG_ERR,
					     _("Error reading HTTP response body\n"));
				openconnect_close_https(vpninfo, 0);
				free(body);
				return -EINVAL;
			}
			done += i;
		}
	} else if (bodylen == BODY_CHUNKED) {
		/* ... else, chunked */
		while ((i = vpninfo->ssl_gets(vpninfo, buf, sizeof(buf)))) {
			int lastchunk = 0;
			long chunklen;

			if (i < 0) {
				vpn_progress(vpninfo, PRG_ERR,
					     _("Error fetching chunk header\n"));
				return i;
			}
			chunklen = strtol(buf, NULL, 16);
			if (!chunklen) {
				lastchunk = 1;
				goto skip;
			}
			if (chunklen < 0) {
				vpn_progress(vpninfo, PRG_ERR,
					     _("HTTP chunk length is negative (%ld)\n"), chunklen);
				openconnect_close_https(vpninfo, 0);
				return -EINVAL;
			}
			if (chunklen >= INT_MAX) {
				vpn_progress(vpninfo, PRG_ERR,
					     _("HTTP chunk length is too large (%ld)\n"), chunklen);
				openconnect_close_https(vpninfo, 0);
				return -EINVAL;
			}
			realloc_inplace(body, done + chunklen + 1);
			if (!body)
				return -ENOMEM;
			while (chunklen) {
				i = vpninfo->ssl_read(vpninfo, body + done, chunklen);
				if (i < 0) {
					vpn_progress(vpninfo, PRG_ERR,
						     _("Error reading HTTP response body\n"));
					free(body);
					return -EINVAL;
				}
				chunklen -= i;
				done += i;
			}
		skip:
			if ((i = vpninfo->ssl_gets(vpninfo, buf, sizeof(buf)))) {
				if (i < 0) {
					vpn_progress(vpninfo, PRG_ERR,
						     _("Error fetching HTTP response body\n"));
				} else {
					vpn_progress(vpninfo, PRG_ERR,
						     _("Error in chunked decoding. Expected '', got: '%s'"),
						     buf);
				}
				free(body);
				return -EINVAL;
			}

			if (lastchunk)
				break;
		}
	} else if (bodylen == BODY_HTTP10) {
		if (!closeconn) {
			vpn_progress(vpninfo, PRG_ERR,
				     _("Cannot receive HTTP 1.0 body without closing connection\n"));
			openconnect_close_https(vpninfo, 0);
			return -EINVAL;
		}

		/* HTTP 1.0 response. Just eat all we can in 16KiB chunks */
		while (1) {
			realloc_inplace(body, done + 16384);
			if (!body)
				return -ENOMEM;
			i = vpninfo->ssl_read(vpninfo, body + done, 16384);
			if (i > 0) {
				/* Got more data */
				done += i;
			} else if (i < 0) {
				/* Error */
				free(body);
				openconnect_close_https(vpninfo, 0);
				return i;
			} else {
				/* Connection closed. Reduce allocation to just what we need */
				realloc_inplace(body, done + 1);
				if (!body)
					return -ENOMEM;
				break;
			}
		}
	}

	if (closeconn || vpninfo->no_http_keepalive)
		openconnect_close_https(vpninfo, 0);

	if (body)
		body[done] = 0;
	*body_ret = body;
	return done;
}