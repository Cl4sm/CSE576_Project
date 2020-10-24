static int reload_pem_cert(struct openconnect_info *vpninfo)
{
	BIO *b = BIO_new(BIO_s_file_internal());
	char buf[200];

	if (!b)
		return -ENOMEM;

	if (BIO_read_filename(b, vpninfo->cert) <= 0) {
	err:
		BIO_free(b);
		vpn_progress(vpninfo, PRG_ERR,
			     _("Failed to reload X509 cert for expiry check\n"));
		openconnect_report_ssl_errors(vpninfo);
		return -EIO;
	}
	vpninfo->cert_x509 = PEM_read_bio_X509_AUX(b, NULL, NULL, NULL);
	BIO_free(b);
	if (!vpninfo->cert_x509)
		goto err;

	X509_NAME_oneline(X509_get_subject_name(vpninfo->cert_x509), buf, sizeof(buf));
	vpn_progress(vpninfo, PRG_INFO,
			     _("Using client certificate '%s'\n"), buf);

	return 0;
}