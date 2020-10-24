static int check_certificate_expiry(struct openconnect_info *vpninfo, gnutls_x509_crt_t cert)
{
	const char *reason = NULL;
	time_t expires = gnutls_x509_crt_get_expiration_time(cert);
	time_t now = time(NULL);

	if (expires == -1) {
		vpn_progress(vpninfo, PRG_ERR,
			     _("Could not extract expiration time of certificate\n"));
		return -EINVAL;
	}

	if (expires < now)
		reason = _("Client certificate has expired at");
	else if (expires < now + vpninfo->cert_expire_warning)
		reason = _("Client certificate expires soon at");

	if (reason) {
		char buf[80];
#ifdef _WIN32
		/*
		 * Windows doesn't have gmtime_r but apparently its gmtime()
		 * *is* thread-safe because it uses a per-thread static buffer.
		 * cf. http://sourceforge.net/p/mingw/bugs/1625/
		 *
		 * We also explicitly say 'GMT' because %Z would give us the
		 * Microsoft stupidity "GMT Standard Time". Which is not only
		 * silly, but also ambiguous because Windows actually says that
		 * even when it means British Summer Time (GMT+1). And having
		 * used gmtime() we really *are* giving the time in GMT.
		 */
		struct tm *tm = gmtime(&expires);
		strftime(buf, 80, "%a, %d %b %Y %H:%M:%S GMT", tm);
#else
		struct tm tm;

		gmtime_r(&expires, &tm);
		strftime(buf, 80, "%a, %d %b %Y %T %Z", &tm);
#endif
		vpn_progress(vpninfo, PRG_ERR, "%s: %s\n", reason, buf);
	}
	return 0;
}