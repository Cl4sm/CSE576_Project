struct dclist *get_dc_list(unsigned int logopt, const char *uri)
{
	LDAPURLDesc *ludlist = NULL;
	LDAPURLDesc **ludp;
	unsigned int min_ttl = MAX_TTL;
	struct dclist *dclist = NULL;;
	char buf[MAX_ERR_BUF];
	char *dn_uri, *esc_uri;
	char *domain;
	char *list;
	int ret;

	if (strcmp(uri, "ldap:///") && strcmp(uri, "ldaps:///")) {
		dn_uri = strdup(uri);
		if (!dn_uri) {
			char *estr = strerror_r(errno, buf, MAX_ERR_BUF);
			error(logopt, "strdup: %s", estr);
			return NULL;
		}
	} else {
		char *dnsdomain;
		char *hdn;

		dnsdomain = getdnsdomainname(logopt);
		if (!dnsdomain) {
			error(logopt, "failed to get dns domainname");
			return NULL;
		}

		if (ldap_domain2dn(dnsdomain, &hdn) || hdn == NULL) {
			error(logopt,
			      "Could not turn domain \"%s\" into a dn\n",
			      dnsdomain);
			free(dnsdomain);
			return NULL;
		}
		free(dnsdomain);

		dn_uri = malloc(strlen(uri) + strlen(hdn) + 1);
		if (!dn_uri) {
			char *estr = strerror_r(errno, buf, MAX_ERR_BUF);
			error(logopt, "malloc: %s", estr);
			ber_memfree(hdn);
			return NULL;
		}

		strcpy(dn_uri, uri);
		strcat(dn_uri, hdn);
		ber_memfree(hdn);
	}

	esc_uri = escape_dn_commas(dn_uri);
	if (!esc_uri) {
		error(logopt, "Could not escape commas in uri %s", dn_uri);
		free(dn_uri);
		return NULL;
	}

	ret = ldap_url_parse(esc_uri, &ludlist);
	if (ret != LDAP_URL_SUCCESS) {
		error(logopt, "Could not parse uri %s (%d)", dn_uri, ret);
		free(esc_uri);
		free(dn_uri);
		return NULL;
	}

	free(esc_uri);

	if (!ludlist) {
		error(logopt, "No dn found in uri %s", dn_uri);
		free(dn_uri);
		return NULL;
	}

	free(dn_uri);

	dclist = malloc(sizeof(struct dclist));
	if (!dclist) {
		char *estr = strerror_r(errno, buf, MAX_ERR_BUF);
		error(logopt, "malloc: %s", estr);
		ldap_free_urldesc(ludlist);
		return NULL;
	}
	memset(dclist, 0, sizeof(struct dclist));

	list = NULL;
	for (ludp = &ludlist; *ludp != NULL;) {
		LDAPURLDesc *lud = *ludp;
		struct srv_rr *dcs = NULL;
		unsigned int numdcs = 0;
		size_t req_len, len;
		char *request = NULL;
		char *tmp;
		int i;

		if (!lud->lud_dn && !lud->lud_dn[0] &&
		   (!lud->lud_host || !lud->lud_host[0])) {
			*ludp = lud->lud_next;
			continue;
		}

		domain = NULL;
		if (ldap_dn2domain(lud->lud_dn, &domain) || domain == NULL) {
			error(logopt,
			      "Could not turn dn \"%s\" into a domain",
			      lud->lud_dn);
			*ludp = lud->lud_next;
			continue;
		}

		debug(logopt, "doing lookup of SRV RRs for domain %s", domain);

		req_len = sizeof("_ldap._tcp.") + strlen(domain);
		request = malloc(req_len);
		if (!request) {
			char *estr = strerror_r(errno, buf, MAX_ERR_BUF);
			error(logopt, "malloc: %s", estr);
			goto out_error;
		}

		ret = snprintf(request, req_len, "_ldap._tcp.%s", domain);
		if (ret >= req_len) {
			free(request);
			goto out_error;
		}

		dclist_mutex_lock();
		ret = get_srv_rrs(logopt, request, &dcs, &numdcs);
		if (!ret | !dcs) {
			error(logopt,
			      "DNS SRV query failed for domain %s", domain);
			dclist_mutex_unlock();
			free(request);
			goto out_error;
		}
		dclist_mutex_unlock();
		free(request);

		len = strlen(lud->lud_scheme);
		len += sizeof("://");
		len *= numdcs;

		for (i = 0; i < numdcs; i++) {
			if (dcs[i].ttl > 0 && dcs[i].ttl < min_ttl)
				min_ttl = dcs[i].ttl;
			len += strlen(dcs[i].name);
			if (dcs[i].port > 0)
				len += sizeof(":65535");
		}

		tmp = realloc(list, len);
		if (!tmp) {
			char *estr = strerror_r(errno, buf, MAX_ERR_BUF);
			error(logopt, "realloc: %s", estr);
			free_srv_rrs(dcs, numdcs);
			goto out_error;
		}

		if (!list)
			memset(tmp, 0, len);
		else
			strcat(tmp, " ");

		list = NULL;
		for (i = 0; i < numdcs; i++) {
			if (i > 0)
				strcat(tmp, " ");
			strcat(tmp, lud->lud_scheme);
			strcat(tmp, "://");
			strcat(tmp, dcs[i].name);
			if (dcs[i].port > 0) {
				char port[7];
				ret = snprintf(port, 7, ":%d", dcs[i].port);
				if (ret > 6) {
					error(logopt,
					      "invalid port: %u", dcs[i].port);
					free_srv_rrs(dcs, numdcs);
					free(tmp);
					goto out_error;
				}
				strcat(tmp, port);
			}
		}
		list = tmp;

		*ludp = lud->lud_next;
		ber_memfree(domain);
		free_srv_rrs(dcs, numdcs);
	}

	ldap_free_urldesc(ludlist);

	if (!list)
		goto out_error;

	dclist->expire = time(NULL) + min_ttl;
	dclist->uri = list;

	return dclist;

out_error:
	if (list)
		free(list);
	if (domain)
		ber_memfree(domain);
	ldap_free_urldesc(ludlist);
	free_dclist(dclist);
	return NULL;
}