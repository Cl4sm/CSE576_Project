static LDAP *do_reconnect(unsigned logopt, struct lookup_context *ctxt)
{
	LDAP *ldap = NULL;

	if (ctxt->server || !ctxt->uris) {
		ldap = do_connect(logopt, ctxt->server, ctxt);
#ifdef WITH_SASL
		/* Dispose of the sasl authentication connection and try again. */
		if (!ldap && ctxt->auth_required & LDAP_NEED_AUTH) {
			autofs_sasl_dispose(ctxt);
			ldap = connect_to_server(logopt, ctxt->server, ctxt);
		}
#endif
		return ldap;
	}

	if (ctxt->dclist) {
		ldap = find_dc_server(logopt, ctxt->dclist->uri, ctxt);
		if (ldap)
			return ldap;
	}

	uris_mutex_lock(ctxt);
	if (ctxt->dclist) {
		if (!ldap || ctxt->dclist->expire < time(NULL)) {
			free_dclist(ctxt->dclist);
			ctxt->dclist = NULL;
		}
		/* Make sure we don't skip the domain spec */
		ctxt->uri = NULL;
		uris_mutex_unlock(ctxt);
		goto find_server;
	}
	uris_mutex_unlock(ctxt);

	if (!ctxt->uri)
		goto find_server;

	ldap = do_connect(logopt, ctxt->uri->uri, ctxt);
#ifdef WITH_SASL
	/*
	 * Dispose of the sasl authentication connection and try the
	 * current server again before trying other servers in the list.
	 */
	if (!ldap && ctxt->auth_required & LDAP_NEED_AUTH) {
		autofs_sasl_dispose(ctxt);
		ldap = connect_to_server(logopt, ctxt->uri->uri, ctxt);
	}
#endif
	if (ldap)
		return ldap;

	/* Failed to connect, try to find a new server */

find_server:
#ifdef WITH_SASL
	autofs_sasl_dispose(ctxt);
#endif

	/* Current server failed, try the rest or dc connection */
	ldap = find_server(logopt, ctxt);
	if (!ldap)
		error(logopt, MODPREFIX "failed to find available server");

	return ldap;
}