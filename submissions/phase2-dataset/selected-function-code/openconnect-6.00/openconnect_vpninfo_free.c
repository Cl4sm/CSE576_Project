void openconnect_vpninfo_free(struct openconnect_info *vpninfo)
{
	openconnect_close_https(vpninfo, 1);
	dtls_shutdown(vpninfo);
	if (vpninfo->cmd_fd_write != -1) {
		close(vpninfo->cmd_fd);
		close(vpninfo->cmd_fd_write);
	}
#ifdef _WIN32
	if (vpninfo->cmd_event)
		CloseHandle(vpninfo->cmd_event);
	if (vpninfo->ssl_event)
		CloseHandle(vpninfo->ssl_event);
	if (vpninfo->dtls_event)
		CloseHandle(vpninfo->dtls_event);
#endif
	free(vpninfo->peer_addr);
	free_optlist(vpninfo->cookies);
	free_optlist(vpninfo->cstp_options);
	free_optlist(vpninfo->dtls_options);
	cstp_free_splits(vpninfo);
	free(vpninfo->hostname);
	free(vpninfo->urlpath);
	free(vpninfo->redirect_url);
	free(vpninfo->cookie);
	free(vpninfo->proxy_type);
	free(vpninfo->proxy);
	free(vpninfo->proxy_user);
	free(vpninfo->proxy_pass);
	free(vpninfo->vpnc_script);
	free(vpninfo->cafile);
	free(vpninfo->servercert);
	free(vpninfo->ifname);
	free(vpninfo->dtls_cipher);
	free(vpninfo->dtls_addr);

	if (vpninfo->csd_scriptname) {
		unlink(vpninfo->csd_scriptname);
		free(vpninfo->csd_scriptname);
	}
	free(vpninfo->mobile_platform_version);
	free(vpninfo->mobile_device_type);
	free(vpninfo->mobile_device_uniqueid);
	free(vpninfo->csd_token);
	free(vpninfo->csd_ticket);
	free(vpninfo->csd_stuburl);
	free(vpninfo->csd_starturl);
	free(vpninfo->csd_waiturl);
	free(vpninfo->csd_preurl);
	if (vpninfo->opaque_srvdata)
		xmlFreeNode(vpninfo->opaque_srvdata);
	free(vpninfo->profile_url);
	free(vpninfo->profile_sha1);

	/* These are const in openconnect itself, but for consistency of
	   the library API we do take ownership of the strings we're given,
	   and thus we have to free them too. */
	if (vpninfo->cert != vpninfo->sslkey)
		free((void *)vpninfo->sslkey);
	free((void *)vpninfo->cert);
	if (vpninfo->peer_cert) {
#if defined(OPENCONNECT_OPENSSL)
		X509_free(vpninfo->peer_cert);
#elif defined(OPENCONNECT_GNUTLS)
		gnutls_x509_crt_deinit(vpninfo->peer_cert);
#endif
		vpninfo->peer_cert = NULL;
	}
	free(vpninfo->localname);
	free(vpninfo->useragent);
	free(vpninfo->authgroup);
#ifdef HAVE_LIBSTOKEN
	if (vpninfo->stoken_pin)
		free(vpninfo->stoken_pin);
	if (vpninfo->stoken_ctx)
		stoken_destroy(vpninfo->stoken_ctx);
#endif
#ifdef HAVE_LIBOATH
	if (vpninfo->oath_secret)
		oath_done();
#endif

	/* These check strm->state so they are safe to call multiple times */
	inflateEnd(&vpninfo->inflate_strm);
	deflateEnd(&vpninfo->deflate_strm);

	free(vpninfo->deflate_pkt);
	free(vpninfo);
}