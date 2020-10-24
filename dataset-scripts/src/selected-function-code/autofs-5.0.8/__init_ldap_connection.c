LDAP *__init_ldap_connection(unsigned logopt, const char *uri, struct lookup_context *ctxt)
{
	LDAP *ldap = NULL;
	struct timeval timeout     = { ctxt->timeout, 0 };
	struct timeval net_timeout = { ctxt->network_timeout, 0 };
	int rv;

	ctxt->version = 3;

	/* Initialize the LDAP context. */
	rv = ldap_initialize(&ldap, uri);
	if (rv != LDAP_OPT_SUCCESS) {
		info(logopt, MODPREFIX
		     "couldn't initialize LDAP connection to %s",
		     uri ? uri : "default");
		return NULL;
	}

	/* Use LDAPv3 */
	rv = ldap_set_option(ldap, LDAP_OPT_PROTOCOL_VERSION, &ctxt->version);
	if (rv != LDAP_OPT_SUCCESS) {
		/* fall back to LDAPv2 */
		ldap_unbind_ext(ldap, NULL, NULL);
		rv = ldap_initialize(&ldap, uri);
		if (rv != LDAP_OPT_SUCCESS) {
			crit(logopt, MODPREFIX "couldn't initialize LDAP");
			return NULL;
		}
		ctxt->version = 2;
	}


	if (ctxt->timeout != -1) {
		/* Set synchronous call timeout */
		rv = ldap_set_option(ldap, LDAP_OPT_TIMEOUT, &timeout);
		if (rv != LDAP_OPT_SUCCESS)
			info(logopt, MODPREFIX
			     "failed to set synchronous call timeout to %d",
			     timeout.tv_sec);
	}

	/* Sane network timeout */
	rv = ldap_set_option(ldap, LDAP_OPT_NETWORK_TIMEOUT, &net_timeout);
	if (rv != LDAP_OPT_SUCCESS)
		info(logopt, MODPREFIX "failed to set connection timeout to %d",
		     net_timeout.tv_sec);

	if (ctxt->use_tls) {
		if (ctxt->version == 2) {
			if (ctxt->tls_required) {
				error(logopt, MODPREFIX
				    "TLS required but connection is version 2");
				ldap_unbind_ext(ldap, NULL, NULL);
				return NULL;
			}
			return ldap;
		}

		rv = ldap_start_tls_s(ldap, NULL, NULL);
		if (rv != LDAP_SUCCESS) {
			unbind_ldap_connection(logopt, ldap, ctxt);
			if (ctxt->tls_required) {
				error(logopt, MODPREFIX
				      "TLS required but START_TLS failed: %s",
				      ldap_err2string(rv));
				return NULL;
			}
			ctxt->use_tls = LDAP_TLS_DONT_USE;
			ldap = init_ldap_connection(logopt, uri, ctxt);
			if (ldap)
				ctxt->use_tls = LDAP_TLS_INIT;
			return ldap;
		}
		ctxt->use_tls = LDAP_TLS_RELEASE;
	}

	return ldap;
}