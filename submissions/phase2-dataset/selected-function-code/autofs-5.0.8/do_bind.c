static int do_bind(unsigned logopt, LDAP *ldap, const char *uri, struct lookup_context *ctxt)
{
	char *host = NULL, *nhost;
	int rv, need_base = 1;

#ifdef WITH_SASL
	debug(logopt, MODPREFIX "auth_required: %d, sasl_mech %s",
	      ctxt->auth_required, ctxt->sasl_mech);

	if (ctxt->auth_required & LDAP_NEED_AUTH) {
		rv = autofs_sasl_bind(logopt, ldap, ctxt);
		debug(logopt, MODPREFIX "autofs_sasl_bind returned %d", rv);
	} else {
		rv = bind_ldap_simple(logopt, ldap, uri, ctxt);
		debug(logopt, MODPREFIX "ldap simple bind returned %d", rv);
	}
#else
	rv = bind_ldap_simple(logopt, ldap, uri, ctxt);
	debug(logopt, MODPREFIX "ldap simple bind returned %d", rv);
#endif

	if (rv != 0)
		return 0;

	rv = ldap_get_option(ldap, LDAP_OPT_HOST_NAME, &host);
        if (rv != LDAP_SUCCESS || !host) {
		debug(logopt, "failed to get hostname for connection");
		return 0;
	}

	nhost = strdup(host);
	if (!nhost) {
		debug(logopt, "failed to alloc context for hostname");
		return 0;
	}
	ldap_memfree(host);

	if (!ctxt->cur_host) {
		ctxt->cur_host = nhost;
		/* Check if schema defined in conf first time only */
		ctxt->schema = defaults_get_schema();
	} else {
		/* If connection host has changed update */
		if (strcmp(ctxt->cur_host, nhost)) {
			free(ctxt->cur_host);
			ctxt->cur_host = nhost;
		} else {
			free(nhost);
			need_base = 0;
		}
	}

	if (ctxt->schema && ctxt->qdn && !need_base)
		return 1;

	/*
	 * If the schema isn't defined in the configuration then check for
	 * presence of a map dn with a the common schema. Then calculate the
	 * base dn for searches.
	 */
	if (!ctxt->schema) {
		if (!find_query_dn(logopt, ldap, ctxt)) {
			warn(logopt,
			      MODPREFIX "failed to find valid query dn");
			return 0;
		}
	} else {
		const char *class = ctxt->schema->map_class;
		const char *key = ctxt->schema->map_attr;
		if (!get_query_dn(logopt, ldap, ctxt, class, key)) {
			error(logopt, MODPREFIX "failed to get query dn");
			return 0;
		}
	}

	return 1;
}