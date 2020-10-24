static int pem_pw_cb(char *buf, int len, int w, void *v)
{
	struct openconnect_info *vpninfo = v;
	char *pass = NULL;
	int plen;

	if (vpninfo->cert_password) {
		pass = vpninfo->cert_password;
		vpninfo->cert_password = NULL;
	} else if (request_passphrase(vpninfo, "openconnect_pem",
				      &pass, _("Enter PEM pass phrase:")))
		return -1;

	plen = strlen(pass);

	if (len <= plen) {
		vpn_progress(vpninfo, PRG_ERR,
			     _("PEM password too long (%d >= %d)\n"),
			     plen, len);
		free(pass);
		return -1;
	}

	memcpy(buf, pass, plen+1);
	free(pass);
	return plen;
}