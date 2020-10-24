static int do_https_request(struct openconnect_info *vpninfo, const char *method,
			    const char *request_body_type, const char *request_body,
			    char **form_buf, int fetch_redirect)
{
	struct oc_text_buf *buf;
	int result, buflen;
	int rq_retry;
	int rlen, pad;

 redirected:
	vpninfo->redirect_type = REDIR_TYPE_NONE;

	if (*form_buf) {
		free(*form_buf);
		*form_buf = NULL;
	}

	/*
	 * It would be nice to use cURL for this, but we really need to guarantee
	 * that we'll be using OpenSSL (for the TPM stuff), and it doesn't seem
	 * to have any way to let us provide our own socket read/write functions.
	 * We can only provide a socket _open_ function. Which would require having
	 * a socketpair() and servicing the "other" end of it.
	 *
	 * So we process the HTTP for ourselves...
	 */
	buf = buf_alloc();
	buf_append(buf, "%s /%s HTTP/1.1\r\n", method, vpninfo->urlpath ?: "");
	add_common_headers(vpninfo, buf);

	if (request_body_type) {
		rlen = strlen(request_body);

		/* force body length to be a multiple of 64, to avoid leaking
		 * password length. */
		pad = 64*(1+rlen/64) - rlen;
		buf_append(buf, "X-Pad: %0*d\r\n", pad, 0);

		buf_append(buf, "Content-Type: %s\r\n", request_body_type);
		buf_append(buf, "Content-Length: %d\r\n", (int)rlen);
	}
	buf_append(buf, "\r\n");

	if (request_body_type)
		buf_append(buf, "%s", request_body);

	if (vpninfo->port == 443)
		vpn_progress(vpninfo, PRG_INFO, "%s https://%s/%s\n",
			     method, vpninfo->hostname,
			     vpninfo->urlpath ?: "");
	else
		vpn_progress(vpninfo, PRG_INFO, "%s https://%s:%d/%s\n",
			     method, vpninfo->hostname, vpninfo->port,
			     vpninfo->urlpath ?: "");

	if (buf_error(buf))
		return buf_free(buf);

 retry:
	if (openconnect_https_connected(vpninfo)) {
		/* The session is already connected. If we get a failure on
		* *sending* the request, try it again immediately with a new
		* connection. */
		rq_retry = 1;
	} else {
		rq_retry = 0;
		if ((result = openconnect_open_https(vpninfo))) {
			vpn_progress(vpninfo, PRG_ERR,
				     _("Failed to open HTTPS connection to %s\n"),
				     vpninfo->hostname);
			buf_free(buf);
			return result;
		}
	}

	if (vpninfo->dump_http_traffic)
		dump_buf(vpninfo, '>', buf->data);

	result = vpninfo->ssl_write(vpninfo, buf->data, buf->pos);
	if (rq_retry && result < 0) {
		openconnect_close_https(vpninfo, 0);
		goto retry;
	}
	buf_free(buf);
	if (result < 0)
		return result;

	buflen = process_http_response(vpninfo, &result, 0, NULL, form_buf);
	if (buflen < 0) {
		/* We'll already have complained about whatever offended us */
		return buflen;
	}
	if (vpninfo->dump_http_traffic && *form_buf)
		dump_buf(vpninfo, '<', *form_buf);

	if (result != 200 && vpninfo->redirect_url) {
		result = handle_redirect(vpninfo);
		if (result == 0) {
			if (!fetch_redirect)
				return 0;
			goto redirected;
		}
		goto out;
	}
	if (!*form_buf || result != 200) {
		vpn_progress(vpninfo, PRG_ERR,
			     _("Unexpected %d result from server\n"),
			     result);
		result = -EINVAL;
		goto out;
	}

	return buflen;

 out:
	free(*form_buf);
	*form_buf = NULL;
	return result;
}