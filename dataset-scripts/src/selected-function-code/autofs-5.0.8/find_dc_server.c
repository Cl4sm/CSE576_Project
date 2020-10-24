static LDAP *find_dc_server(unsigned logopt, const char *uri, struct lookup_context *ctxt)
{
	char *str, *tok, *ptr = NULL;
	LDAP *ldap = NULL;

	str = strdup(uri);
	if (!str)
		return NULL;

	tok = strtok_r(str, " ", &ptr);
	while (tok) {
		const char *this = (const char *) tok;
		debug(logopt, "trying server uri %s", this);
		ldap = connect_to_server(logopt, this, ctxt);
		if (ldap) {
			info(logopt, "connected to uri %s", this);
			free(str);
			return ldap;
		}
		tok = strtok_r(NULL, " ", &ptr);
	}

	free(str);

	return NULL;
}