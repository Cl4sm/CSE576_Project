static int process_http_proxy(struct openconnect_info *vpninfo)
{
	char *resp = NULL;
	int resplen;
	struct oc_text_buf *reqbuf;
	int result;
	int auth = 0;

	vpn_progress(vpninfo, PRG_INFO,
		     _("Requesting HTTP proxy connection to %s:%d\n"),
		     vpninfo->hostname, vpninfo->port);

 retry:
	reqbuf = buf_alloc();
	buf_append(reqbuf, "CONNECT %s:%d HTTP/1.1\r\n", vpninfo->hostname, vpninfo->port);
	buf_append(reqbuf, "Host: %s\r\n", vpninfo->hostname);
	buf_append(reqbuf, "User-Agent: %s\r\n", vpninfo->useragent);
	buf_append(reqbuf, "Proxy-Connection: keep-alive\r\n");
	buf_append(reqbuf, "Connection: keep-alive\r\n");
	buf_append(reqbuf, "Accept-Encoding: identity\r\n");
	if (auth) {
		int i;

		result = proxy_authorization(vpninfo, reqbuf);
		if (result) {
			buf_free(reqbuf);
			return result;
		}
		/* Forget existing challenges */
		for (i = 0; i < sizeof(auth_methods) / sizeof(auth_methods[0]); i++)
			clear_auth_state(vpninfo, &auth_methods[i], 0);
	}
	buf_append(reqbuf, "\r\n");

	if (buf_error(reqbuf))
		return buf_free(reqbuf);

	if (vpninfo->dump_http_traffic)
		dump_buf(vpninfo, '>', reqbuf->data);

	result = proxy_write(vpninfo, reqbuf->data, reqbuf->pos);
	buf_free(reqbuf);

	if (result < 0) {
		vpn_progress(vpninfo, PRG_ERR,
			     _("Sending proxy request failed: %s\n"),
			     strerror(-result));
		return result;
	}

	resplen = process_http_response(vpninfo, &result, 1, proxy_hdrs, &resp);
	if (resplen < 0)
		return -EINVAL;

	if (resp) {
		free(resp);
		resp = NULL;
	}

	if (result == 407) {
		auth = 1;
		goto retry;
	}

	if (result == 200)
		return 0;

	vpn_progress(vpninfo, PRG_ERR,
		     _("Proxy CONNECT request failed: %d\n"), result);
	return -EIO;
}