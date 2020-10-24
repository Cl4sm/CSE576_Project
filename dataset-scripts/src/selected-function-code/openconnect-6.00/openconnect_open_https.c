int openconnect_open_https(struct openconnect_info *vpninfo)
{
	int ssl_sock = -1;
	int err;
	const char * prio;

	if (vpninfo->https_sess)
		return 0;

	ssl_sock = connect_https_socket(vpninfo);
	if (ssl_sock < 0)
		return ssl_sock;

	if (!vpninfo->https_cred) {
		gnutls_certificate_allocate_credentials(&vpninfo->https_cred);
#ifdef HAVE_GNUTLS_CERTIFICATE_SET_X509_SYSTEM_TRUST
		gnutls_certificate_set_x509_system_trust(vpninfo->https_cred);
#else
		gnutls_certificate_set_x509_trust_file(vpninfo->https_cred,
						       DEFAULT_SYSTEM_CAFILE,
						       GNUTLS_X509_FMT_PEM);
#endif
		gnutls_certificate_set_verify_function(vpninfo->https_cred,
						       verify_peer);

#ifdef ANDROID_KEYSTORE
		if (vpninfo->cafile && !strncmp(vpninfo->cafile, "keystore:", 9)) {
			gnutls_datum_t datum;
			unsigned int nr_certs;

			err = load_datum(vpninfo, &datum, vpninfo->cafile);
			if (err < 0) {
				gnutls_certificate_free_credentials(vpninfo->https_cred);
				vpninfo->https_cred = NULL;
				return err;
			}

			/* For GnuTLS 3.x We should use gnutls_x509_crt_list_import2() */
			nr_certs = count_x509_certificates(&datum);
			if (nr_certs) {
				gnutls_x509_crt_t *certs;
				int i;

				certs = calloc(nr_certs, sizeof(*certs));
				if (!certs) {
					vpn_progress(vpninfo, PRG_ERR,
						     _("Failed to allocate memory for cafile certs\n"));
					gnutls_free(datum.data);
					gnutls_certificate_free_credentials(vpninfo->https_cred);
					vpninfo->https_cred = NULL;
					closesocket(ssl_sock);
					return -ENOMEM;
				}
				err = gnutls_x509_crt_list_import(certs, &nr_certs, &datum,
								  GNUTLS_X509_FMT_PEM, 0);
				gnutls_free(datum.data);
				if (err >= 0) {
					nr_certs = err;
					err = gnutls_certificate_set_x509_trust(vpninfo->https_cred,
										certs, nr_certs);
				}
				for (i = 0; i < nr_certs; i++)
					gnutls_x509_crt_deinit(certs[i]);
				free(certs);
				if (err < 0) {
					/* From crt_list_import or set_x509_trust */
					vpn_progress(vpninfo, PRG_ERR,
						     _("Failed to read certs from cafile: %s\n"),
						     gnutls_strerror(err));
					gnutls_certificate_free_credentials(vpninfo->https_cred);
					vpninfo->https_cred = NULL;
					closesocket(ssl_sock);
					return -EINVAL;
				}
			}
		} else
#endif
		if (vpninfo->cafile) {
			err = gnutls_certificate_set_x509_trust_file(vpninfo->https_cred,
								     vpninfo->cafile,
								     GNUTLS_X509_FMT_PEM);
			if (err < 0) {
				vpn_progress(vpninfo, PRG_ERR,
					     _("Failed to open CA file '%s': %s\n"),
					     vpninfo->cafile, gnutls_strerror(err));
				gnutls_certificate_free_credentials(vpninfo->https_cred);
				vpninfo->https_cred = NULL;
				closesocket(ssl_sock);
				return -EINVAL;
			}
		}

		if (vpninfo->cert) {
			err = load_certificate(vpninfo);
			if (err) {
				vpn_progress(vpninfo, PRG_ERR,
					     _("Loading certificate failed. Aborting.\n"));
				gnutls_certificate_free_credentials(vpninfo->https_cred);
				vpninfo->https_cred = NULL;
				closesocket(ssl_sock);
				return err;
			}
		}
	}
	gnutls_init(&vpninfo->https_sess, GNUTLS_CLIENT);
	gnutls_session_set_ptr(vpninfo->https_sess, (void *) vpninfo);
#if defined(HAVE_TROUSERS) && !defined(HAVE_GNUTLS_CERTIFICATE_SET_KEY)
	if (vpninfo->my_pkey == OPENCONNECT_TPM_PKEY)
		gnutls_sign_callback_set(vpninfo->https_sess, gtls2_tpm_sign_cb, vpninfo);
#endif
	/* We depend on 3.2.9 because that has the workaround for the
	   obnoxious F5 firewall that drops packets of certain sizes */
	if (gnutls_check_version("3.2.9") &&
	    string_is_hostname(vpninfo->hostname))
		gnutls_server_name_set(vpninfo->https_sess, GNUTLS_NAME_DNS,
				       vpninfo->hostname,
				       strlen(vpninfo->hostname));

	if (vpninfo->pfs) {
		prio = DEFAULT_PRIO":-RSA";
	} else {
		prio = DEFAULT_PRIO;
	}

	err = gnutls_priority_set_direct(vpninfo->https_sess,
					prio, NULL);
	if (err) {
		vpn_progress(vpninfo, PRG_ERR,
			     _("Failed to set TLS priority string: %s\n"),
			     gnutls_strerror(err));
		gnutls_deinit(vpninfo->https_sess);
		vpninfo->https_sess = NULL;
		closesocket(ssl_sock);
		return -EIO;
	}

	gnutls_record_disable_padding(vpninfo->https_sess);
	gnutls_credentials_set(vpninfo->https_sess, GNUTLS_CRD_CERTIFICATE, vpninfo->https_cred);
	gnutls_transport_set_ptr(vpninfo->https_sess,(gnutls_transport_ptr_t)(intptr_t)ssl_sock);

	vpn_progress(vpninfo, PRG_INFO, _("SSL negotiation with %s\n"),
		     vpninfo->hostname);


	err = cstp_handshake(vpninfo, 1);
	if (err)
		return err;

	vpninfo->ssl_fd = ssl_sock;

	vpninfo->ssl_read = openconnect_gnutls_read;
	vpninfo->ssl_write = openconnect_gnutls_write;
	vpninfo->ssl_gets = openconnect_gnutls_gets;

	return 0;
}