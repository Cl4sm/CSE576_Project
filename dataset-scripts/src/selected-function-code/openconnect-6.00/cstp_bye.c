int cstp_bye(struct openconnect_info *vpninfo, const char *reason)
{
	unsigned char *bye_pkt;
	int reason_len;

	/* already lost connection? */
#if defined(OPENCONNECT_OPENSSL)
	if (!vpninfo->https_ssl)
		return 0;
#elif defined(OPENCONNECT_GNUTLS)
	if (!vpninfo->https_sess)
		return 0;
#endif

	reason_len = strlen(reason);
	bye_pkt = malloc(reason_len + 9);
	if (!bye_pkt)
		return -ENOMEM;

	memcpy(bye_pkt, data_hdr, 8);
	memcpy(bye_pkt + 9, reason, reason_len);

	bye_pkt[4] = (reason_len + 1) >> 8;
	bye_pkt[5] = (reason_len + 1) & 0xff;
	bye_pkt[6] = AC_PKT_DISCONN;
	bye_pkt[8] = 0xb0;

	vpn_progress(vpninfo, PRG_INFO,
		     _("Send BYE packet: %s\n"), reason);

	cstp_write(vpninfo, bye_pkt, reason_len + 9);
	free(bye_pkt);

	return 0;
}