int openconnect_make_cstp_connection(struct openconnect_info *vpninfo)
{
	int ret;

	/* This needs to be done before openconnect_setup_dtls() because it's
	   sent with the CSTP CONNECT handshake. Even if we don't end up doing
	   DTLS. */
	if (vpninfo->dtls_state == DTLS_NOSECRET) {
		if (openconnect_random(vpninfo->dtls_secret, sizeof(vpninfo->dtls_secret)))
			return -EINVAL;
		/* The application will later call openconnect_setup_dtls() */
		vpninfo->dtls_state = DTLS_DISABLED;
	}

	ret = openconnect_open_https(vpninfo);
	if (ret)
		return ret;

	if (vpninfo->deflate) {
		vpninfo->deflate_adler32 = 1;
		vpninfo->inflate_adler32 = 1;

		if (inflateInit2(&vpninfo->inflate_strm, -12) ||
		    deflateInit2(&vpninfo->deflate_strm, Z_DEFAULT_COMPRESSION,
				 Z_DEFLATED, -12, 9, Z_DEFAULT_STRATEGY)) {
			vpn_progress(vpninfo, PRG_ERR, _("Compression setup failed\n"));
			vpninfo->deflate = 0;
		}

		if (!vpninfo->deflate_pkt) {
			vpninfo->deflate_pkt = malloc(sizeof(struct pkt) + 2048);
			if (!vpninfo->deflate_pkt) {
				vpn_progress(vpninfo, PRG_ERR,
					     _("Allocation of deflate buffer failed\n"));
				inflateEnd(&vpninfo->inflate_strm);
				deflateEnd(&vpninfo->deflate_strm);
				vpninfo->deflate = 0;
			} else {
				memset(vpninfo->deflate_pkt, 0, sizeof(struct pkt));
				memcpy(vpninfo->deflate_pkt->hdr, data_hdr, 8);
				vpninfo->deflate_pkt->hdr[6] = AC_PKT_COMPRESSED;
			}
		}
	}

	ret = start_cstp_connection(vpninfo);
	if (ret < 0)
		openconnect_close_https(vpninfo, 0);
	return ret;
}