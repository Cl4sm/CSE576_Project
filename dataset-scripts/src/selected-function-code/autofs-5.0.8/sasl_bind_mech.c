sasl_conn_t *
sasl_bind_mech(unsigned logopt, LDAP *ldap, struct lookup_context *ctxt, const char *mech)
{
	sasl_conn_t *conn;
	char *tmp, *host = NULL;
	const char *clientout;
	unsigned int clientoutlen;
	const char *chosen_mech;
	int result;

	if (!strncmp(mech, "GSSAPI", 6)) {
		if (ctxt->client_cc)
			result = sasl_do_kinit_ext_cc(logopt, ctxt);
		else
			result = sasl_do_kinit(logopt, ctxt);
		if (result != 0)
			return NULL;
	}

	debug(logopt, "Attempting sasl bind with mechanism %s", mech);

	result = ldap_get_option(ldap, LDAP_OPT_HOST_NAME, &host);
	if (result != LDAP_OPT_SUCCESS || !host) {
		debug(logopt, "failed to get hostname for connection");
		return NULL;
	}

	/*
	 * We need a host name to start the client.
	 * But the ldap library can return a list of host names so
	 * just use the first one.
	 */
	if ((tmp = strchr(host, ' ')))
		*tmp = '\0';
	if ((tmp = strrchr(host, ':'))) {
		if (*(tmp - 1) != ']') {
			*tmp = '\0';
			tmp = host;
		} else {
			*(tmp - 1) = '\0';
			tmp = host;
			if (*tmp == '[')
				tmp++;
		}
	}

	/* Create a new authentication context for the service. */
	result = sasl_client_new("ldap", tmp, NULL, NULL, NULL, 0, &conn);
	if (result != SASL_OK) {
		error(logopt, "sasl_client_new failed with error %d",
		      result);
		ldap_memfree(host);
		return NULL;
	}

	chosen_mech = NULL;
	result = sasl_client_start(conn, mech, NULL,
				&clientout, &clientoutlen, &chosen_mech);

	/* OK and CONTINUE are the only non-fatal return codes here. */
	if ((result != SASL_OK) && (result != SASL_CONTINUE)) {
		warn(logopt, "sasl_client_start failed for %s", host);
		debug(logopt, "sasl_client_start: %s", sasl_errdetail(conn));
		ldap_memfree(host);
		sasl_dispose(&conn);
		return NULL;
	}

	result = do_sasl_bind(logopt, ldap, conn,
			 &clientout, &clientoutlen, chosen_mech, result);
	if (result == 0) {
		ldap_memfree(host);
		debug(logopt, "sasl bind with mechanism %s succeeded",
		      chosen_mech);
		return conn;
	}

	info(logopt, "sasl bind with mechanism %s failed", mech);

	/* sasl bind failed */
	ldap_memfree(host);
	sasl_dispose(&conn);

	return NULL;
}