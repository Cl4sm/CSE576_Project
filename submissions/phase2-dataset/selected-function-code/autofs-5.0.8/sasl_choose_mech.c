sasl_conn_t *
sasl_choose_mech(unsigned logopt, LDAP *ldap, struct lookup_context *ctxt)
{
	sasl_conn_t *conn = NULL;
	int authenticated;
	int i;
	char **mechanisms;

	mechanisms = get_server_SASL_mechanisms(logopt, ldap);
	if (!mechanisms)
		return NULL;

	/* Try each supported mechanism in turn. */
	authenticated = 0;
	for (i = 0; mechanisms[i] != NULL; i++) {
		/*
		 *  This routine is called if there is no configured
		 *  mechanism.  As such, we can skip over any auth
		 *  mechanisms that require user credentials.  These include
		 *  PLAIN, LOGIN, and DIGEST-MD5.
		 */
		if (authtype_requires_creds(mechanisms[i]))
			continue;

		conn = sasl_bind_mech(logopt, ldap, ctxt, mechanisms[i]);
		if (conn) {
			ctxt->sasl_mech = strdup(mechanisms[i]);
			if (!ctxt->sasl_mech) {
				crit(logopt, "Successfully authenticated with "
				     "mechanism %s, but failed to allocate "
				     "memory to hold the mechanism type.",
				     mechanisms[i]);
				sasl_dispose(&conn);
				ldap_value_free(mechanisms);
				return NULL;
			}
			authenticated = 1;
			break;
		}
		debug(logopt, "Failed to authenticate with mech %s",
		      mechanisms[i]);
	}

	debug(logopt, "authenticated: %d, sasl_mech: %s",
	      authenticated, ctxt->sasl_mech);

	ldap_value_free(mechanisms);
	return conn;
}