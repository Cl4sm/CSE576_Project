static int get_query_dn(unsigned logopt, LDAP *ldap, struct lookup_context *ctxt, const char *class, const char *key)
{
	char buf[MAX_ERR_BUF];
	char *query, *dn, *qdn;
	LDAPMessage *result = NULL, *e;
	char *attrs[2];
	struct berval **value;
	int scope;
	int rv, l;

	attrs[0] = (char *) key;
	attrs[1] = NULL;

	if (!ctxt->mapname && !ctxt->base) {
		error(logopt, MODPREFIX "no master map to lookup");
		return 0;
	}

	/* Build a query string. */
	l = strlen("(objectclass=)") + strlen(class) + 1;
	if (ctxt->mapname)
		l += strlen(key) + strlen(ctxt->mapname) + strlen("(&(=))");

	query = malloc(l);
	if (query == NULL) {
		char *estr = strerror_r(errno, buf, sizeof(buf));
		crit(logopt, MODPREFIX "malloc: %s", estr);
		return NSS_STATUS_UNAVAIL;
	}

	/*
	 * If we have a master mapname construct a query using it
	 * otherwise assume the base dn will catch it.
	 */
	if (ctxt->mapname) {
		if (sprintf(query, "(&(objectclass=%s)(%s=%.*s))", class,
		     key, (int) strlen(ctxt->mapname), ctxt->mapname) >= l) {
			debug(logopt,
			      MODPREFIX "error forming query string");
			free(query);
			return 0;
		}
		scope = LDAP_SCOPE_SUBTREE;
	} else {
		if (sprintf(query, "(objectclass=%s)", class) >= l) {
			debug(logopt,
			      MODPREFIX "error forming query string");
			free(query);
			return 0;
		}
		scope = LDAP_SCOPE_SUBTREE;
	}

	dn = NULL;
	if (!ctxt->sdns) {
		rv = ldap_search_s(ldap, ctxt->base,
				   scope, query, attrs, 0, &result);
		if ((rv != LDAP_SUCCESS) || !result) {
			error(logopt,
			      MODPREFIX "query failed for %s: %s",
			      query, ldap_err2string(rv));
			if (result)
				ldap_msgfree(result);
			free(query);
			return 0;
		}

		e = ldap_first_entry(ldap, result);
		if (e && (value = ldap_get_values_len(ldap, e, key))) {
			ldap_value_free_len(value);
			dn = ldap_get_dn(ldap, e);
			debug(logopt, MODPREFIX "found query dn %s", dn);
		} else {
			debug(logopt,
			      MODPREFIX "query succeeded, no matches for %s",
			      query);
			ldap_msgfree(result);
			free(query);
			return 0;
		}
	} else {
		struct ldap_searchdn *this = ctxt->sdns;

		debug(logopt, MODPREFIX "check search base list");

		result = NULL;
		while (this) {
			rv = ldap_search_s(ldap, this->basedn,
					   scope, query, attrs, 0, &result);
			if ((rv == LDAP_SUCCESS) && result) {
				debug(logopt, MODPREFIX
				      "found search base under %s",
				      this->basedn);

				e = ldap_first_entry(ldap, result);
				if (e && (value = ldap_get_values_len(ldap, e, key))) {
					ldap_value_free_len(value);
					dn = ldap_get_dn(ldap, e);
					debug(logopt, MODPREFIX "found query dn %s", dn);
					break;
				} else {
					debug(logopt,
					      MODPREFIX "query succeeded, no matches for %s",
					      query);
					ldap_msgfree(result);
					result = NULL;
				}
			} else {
				error(logopt,
				      MODPREFIX "query failed for search dn %s: %s",
				      this->basedn, ldap_err2string(rv));
				if (result) {
					ldap_msgfree(result);
					result = NULL;
				}
			}

			this = this->next;
		}

		if (!result) {
			error(logopt,
			      MODPREFIX "failed to find query dn under search base dns");
			free(query);
			return 0;
		}
	}

	free(query);
	qdn = strdup(dn);
	ldap_memfree(dn);
	ldap_msgfree(result);
	if (!qdn)
		return 0;

	if (ctxt->qdn)
		free(ctxt->qdn);

	ctxt->qdn = qdn;

	return 1;
}