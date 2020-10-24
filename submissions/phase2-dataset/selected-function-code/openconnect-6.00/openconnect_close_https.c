void openconnect_close_https(struct openconnect_info *vpninfo, int final)
{
	if (vpninfo->peer_cert) {
		gnutls_x509_crt_deinit(vpninfo->peer_cert);
		vpninfo->peer_cert = NULL;
	}
	if (vpninfo->https_sess) {
		gnutls_deinit(vpninfo->https_sess);
		vpninfo->https_sess = NULL;
	}
	if (vpninfo->ssl_fd != -1) {
		closesocket(vpninfo->ssl_fd);
		unmonitor_read_fd(vpninfo, ssl);
		unmonitor_write_fd(vpninfo, ssl);
		unmonitor_except_fd(vpninfo, ssl);
		vpninfo->ssl_fd = -1;
	}
	if (final && vpninfo->https_cred) {
		gnutls_certificate_free_credentials(vpninfo->https_cred);
		vpninfo->https_cred = NULL;
#ifdef HAVE_P11KIT
		if ((vpninfo->cert && !strncmp(vpninfo->cert, "pkcs11:", 7)) ||
		    (vpninfo->sslkey && !strncmp(vpninfo->sslkey, "pkcs11:", 7))) {
			char pin_source[40];

			sprintf(pin_source, "openconnect:%p", vpninfo);
			p11_kit_pin_unregister_callback(pin_source, pin_callback, vpninfo);

			while (vpninfo->pin_cache) {
				struct pin_cache *cache = vpninfo->pin_cache;

				free(cache->token);
				memset(cache->pin, 0x5a, strlen(cache->pin));
				free(cache->pin);
				vpninfo->pin_cache = cache->next;
				free(cache);
			}
		}
#endif
#ifdef HAVE_TROUSERS
		if (vpninfo->tpm_key_policy) {
			Tspi_Context_CloseObject(vpninfo->tpm_context, vpninfo->tpm_key_policy);
			vpninfo->tpm_key = 0;
		}
		if (vpninfo->tpm_key) {
			Tspi_Context_CloseObject(vpninfo->tpm_context, vpninfo->tpm_key);
			vpninfo->tpm_key = 0;
		}
		if (vpninfo->srk_policy) {
			Tspi_Context_CloseObject(vpninfo->tpm_context, vpninfo->srk_policy);
			vpninfo->srk_policy = 0;
		}
		if (vpninfo->srk) {
			Tspi_Context_CloseObject(vpninfo->tpm_context, vpninfo->srk);
			vpninfo->srk = 0;
		}
		if (vpninfo->tpm_context) {
			Tspi_Context_Close(vpninfo->tpm_context);
			vpninfo->tpm_context = 0;
		}
#endif
#ifndef HAVE_GNUTLS_CERTIFICATE_SET_KEY
		if (vpninfo->my_pkey && vpninfo->my_pkey != OPENCONNECT_TPM_PKEY) {
			gnutls_privkey_deinit(vpninfo->my_pkey);
			vpninfo->my_pkey = NULL;
			/* my_p11key went with it */
		}
		if (vpninfo->my_certs) {
			int i;
			for (i = 0; i < vpninfo->nr_my_certs; i++)
				if (vpninfo->free_my_certs[i])
					gnutls_x509_crt_deinit(vpninfo->my_certs[i]);
			gnutls_free(vpninfo->my_certs);
			gnutls_free(vpninfo->free_my_certs);
			vpninfo->my_certs = NULL;
			vpninfo->free_my_certs = NULL;
		}
#endif
	}
}