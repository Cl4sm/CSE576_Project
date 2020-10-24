static int ntlm_helper_challenge(struct openconnect_info *vpninfo, struct oc_text_buf *buf)
{
	char helperbuf[4096];
	int len;

	if (!vpninfo->auth[AUTH_TYPE_NTLM].challenge ||
	    write(vpninfo->ntlm_helper_fd, "TT ", 3) != 3 ||
	    write(vpninfo->ntlm_helper_fd, vpninfo->auth[AUTH_TYPE_NTLM].challenge,
		  strlen(vpninfo->auth[AUTH_TYPE_NTLM].challenge)) != strlen(vpninfo->auth[AUTH_TYPE_NTLM].challenge) ||
	    write(vpninfo->ntlm_helper_fd, "\n", 1) != 1) {
	err:
		close(vpninfo->ntlm_helper_fd);
		vpninfo->ntlm_helper_fd = -1;
		return -EAGAIN;
	}
	len = read(vpninfo->ntlm_helper_fd, helperbuf, sizeof(helperbuf));
	if (len < 4 || helperbuf[0] != 'K' || helperbuf[1] != 'K' ||
	    helperbuf[2] != ' ' || helperbuf[len - 1] != '\n') {
		goto err;
	}
	helperbuf[len - 1] = 0;
	buf_append(buf, "Proxy-Authorization: NTLM %s\r\n", helperbuf + 3);

	vpn_progress(vpninfo, PRG_INFO, _("Attempting HTTP NTLM authentication to proxy (single-sign-on)\n"));
	return 0;

}