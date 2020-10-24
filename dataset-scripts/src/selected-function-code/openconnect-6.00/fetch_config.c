static int fetch_config(struct openconnect_info *vpninfo)
{
	struct oc_text_buf *buf;
	char *config_buf = NULL;
	int result, buflen;
	unsigned char local_sha1_bin[SHA1_SIZE];
	char local_sha1_ascii[(SHA1_SIZE * 2)+1];
	int i;

	if (!vpninfo->profile_url || !vpninfo->profile_sha1 || !vpninfo->write_new_config)
		return -ENOENT;

	if (!strncasecmp(vpninfo->xmlsha1, vpninfo->profile_sha1, SHA1_SIZE * 2)) {
		vpn_progress(vpninfo, PRG_TRACE,
			     _("Not downloading XML profile because SHA1 already matches\n"));
		return 0;
	}

	if ((result = openconnect_open_https(vpninfo))) {
		vpn_progress(vpninfo, PRG_ERR,
			     _("Failed to open HTTPS connection to %s\n"),
			     vpninfo->hostname);
		return result;
	}

	buf = buf_alloc();
	buf_append(buf, "GET %s HTTP/1.1\r\n", vpninfo->profile_url);
	add_common_headers(vpninfo, buf);
	if (vpninfo->xmlpost)
		buf_append(buf, "Cookie: webvpn=%s\r\n", vpninfo->cookie);
	buf_append(buf, "\r\n");

	if (buf_error(buf))
		return buf_free(buf);

	if (vpninfo->ssl_write(vpninfo, buf->data, buf->pos) != buf->pos) {
		vpn_progress(vpninfo, PRG_ERR,
			     _("Failed to send GET request for new config\n"));
		buf_free(buf);
		return -EIO;
	}
	buf_free(buf);

	buflen = process_http_response(vpninfo, &result, 0, NULL, &config_buf);
	if (buflen < 0) {
		/* We'll already have complained about whatever offended us */
		return -EINVAL;
	}

	if (result != 200) {
		free(config_buf);
		return -EINVAL;
	}

	openconnect_sha1(local_sha1_bin, config_buf, buflen);

	for (i = 0; i < SHA1_SIZE; i++)
		sprintf(&local_sha1_ascii[i*2], "%02x", local_sha1_bin[i]);

	if (strcasecmp(vpninfo->profile_sha1, local_sha1_ascii)) {
		vpn_progress(vpninfo, PRG_ERR,
			     _("Downloaded config file did not match intended SHA1\n"));
		free(config_buf);
		return -EINVAL;
	}

	vpn_progress(vpninfo, PRG_DEBUG, _("Downloaded new XML profile\n"));

	result = vpninfo->write_new_config(vpninfo->cbdata, config_buf, buflen);
	free(config_buf);
	return result;
}