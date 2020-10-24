static LDAP *find_server(unsigned logopt, struct lookup_context *ctxt)
{
	LDAP *ldap = NULL;
	struct ldap_uri *this;
	struct list_head *p, *first;
	struct dclist *dclist;
	char *uri = NULL;

	uris_mutex_lock(ctxt);
	dclist = ctxt->dclist;
	if (!ctxt->uri)
		first = ctxt->uris;
	else
		first = &ctxt->uri->list;
	uris_mutex_unlock(ctxt);


	/* Try each uri, save point in server list upon success */
	p = first->next;
	while(p != first) {
		/* Skip list head */
		if (p == ctxt->uris) {
			p = p->next;
			continue;
		}
		this = list_entry(p, struct ldap_uri, list);
		if (!strstr(this->uri, ":///")) {
			uri = strdup(this->uri);
			debug(logopt, "trying server uri %s", uri);
			ldap = connect_to_server(logopt, uri, ctxt);
			if (ldap) {
				info(logopt, "connected to uri %s", uri);
				free(uri);
				break;
			}
		} else {
			if (dclist)
				uri = strdup(dclist->uri);
			else {
				struct dclist *tmp;
				tmp = get_dc_list(logopt, this->uri);
				if (!tmp) {
					p = p->next;
					continue;
				}
				dclist = tmp;
				uri = strdup(dclist->uri);
			}
			ldap = find_dc_server(logopt, uri, ctxt);
			if (ldap) {
				free(uri);
				break;
			}
		}
		free(uri);
		uri = NULL;
		if (dclist) {
			free_dclist(dclist);
			dclist = NULL;
		}
		p = p->next;
	}

	uris_mutex_lock(ctxt);
	if (ldap)
		ctxt->uri = this;
	if (dclist) {
		if (!ctxt->dclist)
			ctxt->dclist = dclist;
		else {
			if (ctxt->dclist != dclist) {
				free_dclist(ctxt->dclist);
				ctxt->dclist = dclist;
			}
		}
	}
	uris_mutex_unlock(ctxt);

	return ldap;
}