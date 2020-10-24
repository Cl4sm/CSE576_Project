int bind_ldap_simple(unsigned logopt, LDAP *ldap, const char *uri, struct lookup_context *ctxt)
{
	int rv;

	if (ctxt->auth_required == LDAP_AUTH_USESIMPLE)
		rv = ldap_simple_bind_s(ldap, ctxt->user, ctxt->secret);
	else if (ctxt->version == 2)
		rv = ldap_simple_bind_s(ldap, ctxt->base, NULL);
	else
		rv = ldap_simple_bind_s(ldap, NULL, NULL);

	if (rv != LDAP_SUCCESS) {
		if (!ctxt->uris) {
			crit(logopt, MODPREFIX
			     "Unable to bind to the LDAP server: "
			     "%s, error %s", ctxt->server ? "" : "(default)",
			     ldap_err2string(rv));
		} else {
			info(logopt, MODPREFIX "Unable to bind to the LDAP server: "
			     "%s, error %s", uri, ldap_err2string(rv));
		}
		return -1;
	}

	return 0;
}