static void print_build_opts(void)
{
	const char *comma = ", ", *sep = comma + 1;

#if defined(OPENCONNECT_OPENSSL)
	printf(_("Using OpenSSL. Features present:"));
#elif defined(OPENCONNECT_GNUTLS)
	printf(_("Using GnuTLS. Features present:"));
#endif

	if (openconnect_has_tss_blob_support()) {
		printf("%sTPM", sep);
		sep = comma;
	}
#if defined(OPENCONNECT_OPENSSL) && defined(HAVE_ENGINE)
	else {
		printf("%sTPM (%s)", sep, _("OpenSSL ENGINE not present"));
		sep = comma;
	}
#endif
	if (openconnect_has_pkcs11_support()) {
		printf("%sPKCS#11", sep);
		sep = comma;
	}
	if (openconnect_has_stoken_support()) {
		printf("%sRSA software token", sep);
		sep = comma;
	}
	switch(openconnect_has_oath_support()) {
	case 2:
		printf("%sHOTP software token", sep);
		sep = comma;
	case 1:
		printf("%sTOTP software token", sep);
		sep = comma;
	}

#ifdef HAVE_DTLS
	printf("%sDTLS", sep);
#if defined(OPENCONNECT_GNUTLS) && defined(DTLS_OPENSSL)
	printf(" (%s)", _("using OpenSSL"));
#endif
	printf("\n");
#else
	printf(_("\nWARNING: No DTLS support in this binary. Performance will be impaired.\n"));
#endif
}