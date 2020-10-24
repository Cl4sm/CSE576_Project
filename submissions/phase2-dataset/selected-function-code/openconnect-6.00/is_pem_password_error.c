static int is_pem_password_error(struct openconnect_info *vpninfo)
{
	unsigned long err = ERR_peek_error();

	openconnect_report_ssl_errors(vpninfo);

#ifndef EVP_F_EVP_DECRYPTFINAL_EX
#define EVP_F_EVP_DECRYPTFINAL_EX EVP_F_EVP_DECRYPTFINAL
#endif
	/* If the user fat-fingered the passphrase, try again */
	if (ERR_GET_LIB(err) == ERR_LIB_EVP &&
	    ERR_GET_FUNC(err) == EVP_F_EVP_DECRYPTFINAL_EX &&
	    ERR_GET_REASON(err) == EVP_R_BAD_DECRYPT) {
		vpn_progress(vpninfo, PRG_ERR,
			     _("Loading private key failed (wrong passphrase?)\n"));
		ERR_clear_error();
		return 1;
	}

	vpn_progress(vpninfo, PRG_ERR,
		     _("Loading private key failed (see above errors)\n"));
	return 0;
}