static int verify_peer(gnutls_session_t session)
{
	struct openconnect_info *vpninfo = gnutls_session_get_ptr(session);
	const gnutls_datum_t *cert_list;
	gnutls_x509_crt_t cert;
	unsigned int status, cert_list_size;
	const char *reason = NULL;
	int err;

	if (vpninfo->peer_cert) {
		gnutls_x509_crt_deinit(vpninfo->peer_cert);
		vpninfo->peer_cert = NULL;
	}

	cert_list = gnutls_certificate_get_peers(session, &cert_list_size);
	if (!cert_list) {
		vpn_progress(vpninfo, PRG_ERR, _("Server presented no certificate\n"));
		return GNUTLS_E_CERTIFICATE_ERROR;
	}

	if (vpninfo->servercert) {
		unsigned char sha1bin[SHA1_SIZE];
		char fingerprint[(SHA1_SIZE * 2) + 1];
		int i;

		err = openconnect_sha1(sha1bin, cert_list[0].data, cert_list[0].size);
		if (err) {
			vpn_progress(vpninfo, PRG_ERR,
				     _("Could not calculate SHA1 of server's certificate\n"));
			return GNUTLS_E_CERTIFICATE_ERROR;
		}
		for (i = 0; i < SHA1_SIZE; i++)
			sprintf(&fingerprint[i*2], "%02X", sha1bin[i]);

		if (strcasecmp(vpninfo->servercert, fingerprint)) {
			vpn_progress(vpninfo, PRG_ERR,
				     _("Server SSL certificate didn't match: %s\n"), fingerprint);
			return GNUTLS_E_CERTIFICATE_ERROR;
		}
		return 0;
	}

	err = gnutls_certificate_verify_peers2(session, &status);
	if (err) {
		vpn_progress(vpninfo, PRG_ERR, _("Error checking server cert status\n"));
		return GNUTLS_E_CERTIFICATE_ERROR;
	}

	if (status & GNUTLS_CERT_REVOKED)
		reason = _("certificate revoked");
	else if (status & GNUTLS_CERT_SIGNER_NOT_FOUND)
		reason = _("signer not found");
	else if (status & GNUTLS_CERT_SIGNER_NOT_CA)
		reason = _("signer not a CA certificate");
	else if (status & GNUTLS_CERT_INSECURE_ALGORITHM)
		reason = _("insecure algorithm");
	else if (status & GNUTLS_CERT_NOT_ACTIVATED)
		reason = _("certificate not yet activated");
	else if (status & GNUTLS_CERT_EXPIRED)
		reason = _("certificate expired");
	else if (status & GNUTLS_CERT_INVALID)
		/* If this is set and no other reason, it apparently means
		   that signature verification failed. Not entirely sure
		   why we don't just set a bit for that too. */
		reason = _("signature verification failed");

	err = gnutls_x509_crt_init(&cert);
	if (err) {
		vpn_progress(vpninfo, PRG_ERR, _("Error initialising X509 cert structure\n"));
		return GNUTLS_E_CERTIFICATE_ERROR;
	}

	err = gnutls_x509_crt_import(cert, &cert_list[0], GNUTLS_X509_FMT_DER);
	if (err) {
		vpn_progress(vpninfo, PRG_ERR, _("Error importing server's cert\n"));
		gnutls_x509_crt_deinit(cert);
		return GNUTLS_E_CERTIFICATE_ERROR;
	}

	if (reason)
		goto done;

	if (!gnutls_x509_crt_check_hostname(cert, vpninfo->hostname)) {
		int i, ret;
		unsigned char addrbuf[sizeof(struct in6_addr)];
		unsigned char certaddr[sizeof(struct in6_addr)];
		size_t addrlen = 0, certaddrlen;

		/* gnutls_x509_crt_check_hostname() doesn't cope with IPv6 literals
		   in URI form with surrounding [] so we must check for ourselves. */
		if (vpninfo->hostname[0] == '[' &&
		    vpninfo->hostname[strlen(vpninfo->hostname)-1] == ']') {
			char *p = &vpninfo->hostname[strlen(vpninfo->hostname)-1];
			*p = 0;
			if (inet_pton(AF_INET6, vpninfo->hostname + 1, addrbuf) > 0)
				addrlen = 16;
			*p = ']';
		}
#if GNUTLS_VERSION_NUMBER < 0x030306
		/* And before 3.3.6 it didn't check IP addresses at all. */
		else if (inet_pton(AF_INET, vpninfo->hostname, addrbuf) > 0)
			addrlen = 4;
		else if (inet_pton(AF_INET6, vpninfo->hostname, addrbuf) > 0)
			addrlen = 16;
#endif
		if (!addrlen) {
			/* vpninfo->hostname was not a bare IP address. Nothing to do */
			goto badhost;
		}

		for (i = 0; ; i++) {
			certaddrlen = sizeof(certaddr);
			ret = gnutls_x509_crt_get_subject_alt_name(cert, i, certaddr,
								   &certaddrlen, NULL);
			/* If this happens, it wasn't an IP address. */
			if (ret == GNUTLS_E_SHORT_MEMORY_BUFFER)
				continue;
			if (ret < 0)
				break;
			if (ret != GNUTLS_SAN_IPADDRESS)
				continue;
			if (certaddrlen == addrlen && !memcmp(addrbuf, certaddr, addrlen))
				goto done;
		}
	badhost:
		reason = _("certificate does not match hostname");
	}
 done:
	if (reason) {
		vpn_progress(vpninfo, PRG_INFO,
			     _("Server certificate verify failed: %s\n"),
			     reason);
		if (vpninfo->validate_peer_cert)
			err = vpninfo->validate_peer_cert(vpninfo->cbdata,
							  cert,
							  reason) ? GNUTLS_E_CERTIFICATE_ERROR : 0;
		else
			err = GNUTLS_E_CERTIFICATE_ERROR;
	}

	vpninfo->peer_cert = cert;

	return err;
}