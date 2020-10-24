int
autofs_sasl_bind(unsigned logopt, LDAP *ldap, struct lookup_context *ctxt)
{
	sasl_conn_t *conn = NULL;

	/* If we already have a connection use it */
	if (ctxt->sasl_conn)
		return 0;

	if (ctxt->sasl_mech && !strncmp(ctxt->sasl_mech, "EXTERNAL", 8)) {
		int result;

		debug(logopt,
		      "Attempting sasl bind with mechanism %s",
		      ctxt->sasl_mech);

		result = do_sasl_extern(ldap, ctxt);
		if (result)
			debug(logopt,
			      "Failed to authenticate with mech %s",
			      ctxt->sasl_mech);
		else
			debug(logopt,
			      "sasl bind with mechanism %s succeeded",
			      ctxt->sasl_mech);

		return result;
	}

	sasl_auth_id = ctxt->user;
	sasl_auth_secret = ctxt->secret;

	if (ctxt->auth_required & LDAP_AUTH_AUTODETECT) {
		if (ctxt->sasl_mech) {
			free(ctxt->sasl_mech);
			ctxt->sasl_mech = NULL;
		}
	}

	/*
	 *  If LDAP_AUTH_AUTODETECT is set, it means that there was no
	 *  mechanism specified in the configuration file or auto
	 *  selection has been requested, so try to auto-select an
	 *  auth mechanism.
	 */
	if (ctxt->sasl_mech)
		conn = sasl_bind_mech(logopt, ldap, ctxt, ctxt->sasl_mech);
	else
		conn = sasl_choose_mech(logopt, ldap, ctxt);

	if (!conn)
		return -1;

	ctxt->sasl_conn = conn;
	return 0;
}