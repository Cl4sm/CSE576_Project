static int basic_authorization(struct openconnect_info *vpninfo, struct oc_text_buf *hdrbuf)
{
	struct oc_text_buf *text;

	if (!vpninfo->proxy_user || !vpninfo->proxy_pass)
		return -EINVAL;

	if (!vpninfo->authmethods_set) {
		vpn_progress(vpninfo, PRG_ERR,
			     _("Proxy requested Basic authentication which is disabled by default\n"));
		vpninfo->auth[AUTH_TYPE_BASIC].state = AUTH_FAILED;
		return -EINVAL;
	}

	if (vpninfo->auth[AUTH_TYPE_BASIC].state == AUTH_IN_PROGRESS) {
		vpninfo->auth[AUTH_TYPE_BASIC].state = AUTH_FAILED;
		return -EAGAIN;
	}

	text = buf_alloc();
	buf_append(text, "%s:%s", vpninfo->proxy_user, vpninfo->proxy_pass);
	if (buf_error(text))
		return buf_free(text);

	buf_append(hdrbuf, "Proxy-Authorization: Basic ");
	buf_append_base64(hdrbuf, text->data, text->pos);
	buf_append(hdrbuf, "\r\n");

	memset(text->data, 0, text->pos);
	buf_free(text);

	vpn_progress(vpninfo, PRG_INFO, _("Attempting HTTP Basic authentication to proxy\n"));
	vpninfo->auth[AUTH_TYPE_BASIC].state = AUTH_IN_PROGRESS;
	return 0;
}