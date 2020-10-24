char **
get_server_SASL_mechanisms(unsigned logopt, LDAP *ld)
{
	int ret;
	const char *saslattrlist[] = {"supportedSASLmechanisms", NULL};
	LDAPMessage *results = NULL, *entry;
	char **mechanisms;

	ret = ldap_search_ext_s(ld, "", LDAP_SCOPE_BASE, "(objectclass=*)",
				(char **)saslattrlist, 0,
				NULL, NULL,
				NULL, LDAP_NO_LIMIT, &results);
	if (ret != LDAP_SUCCESS) {
		error(logopt, "%s", ldap_err2string(ret));
		return NULL;
	}

	entry = ldap_first_entry(ld, results);
	if (entry == NULL) {
		/* No root DSE. (!) */
		ldap_msgfree(results);
		debug(logopt,
		      "a lookup of \"supportedSASLmechanisms\" returned "
		      "no results.");
		return NULL;
	}

	mechanisms = ldap_get_values(ld, entry, "supportedSASLmechanisms");
	ldap_msgfree(results);
	if (mechanisms == NULL) {
		/* Well, that was a waste of time. */
		info(logopt, "No SASL authentication mechanisms are supported"
		    " by the LDAP server.");
		return NULL;
	}

	return mechanisms;
}