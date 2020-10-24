static LDAP *do_connect(unsigned logopt, const char *uri, struct lookup_context *ctxt)
{
	LDAP *ldap;

#ifdef WITH_SASL
	if (ctxt->extern_cert && ctxt->extern_key) {
		set_env(logopt, ENV_LDAPTLS_CERT, ctxt->extern_cert);
		set_env(logopt, ENV_LDAPTLS_KEY, ctxt->extern_key);
	}
#endif

	ldap = init_ldap_connection(logopt, uri, ctxt);
	if (ldap) {
		if (!do_bind(logopt, ldap, uri, ctxt)) {
			unbind_ldap_connection(logopt, ldap, ctxt);
			ldap = NULL;
		}
	}

	return ldap;
}