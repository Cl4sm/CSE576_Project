static int read_one_map(struct autofs_point *ap,
			struct lookup_context *ctxt,
			time_t age, int *result_ldap)
{
	struct map_source *source;
	struct ldap_search_params sp;
	char buf[MAX_ERR_BUF];
	char *class, *info, *entry;
	char *attrs[3];
	int rv, l;

	source = ap->entry->current;
	ap->entry->current = NULL;
	master_source_current_signal(ap->entry);

	/*
	 * If we don't need to create directories then there's no use
	 * reading the map. We always need to read the whole map for
	 * direct mounts in order to mount the triggers.
	 */
	if (!(ap->flags & MOUNT_FLAG_GHOST) && ap->type != LKP_DIRECT) {
		debug(ap->logopt, "map read not needed, so not done");
		return NSS_STATUS_SUCCESS;
	}

	sp.ap = ap;
	sp.age = age;

	/* Initialize the LDAP context. */
	sp.ldap = do_reconnect(ap->logopt, ctxt);
	if (!sp.ldap)
		return NSS_STATUS_UNAVAIL;

	class = ctxt->schema->entry_class;
	entry = ctxt->schema->entry_attr;
	info = ctxt->schema->value_attr;

	attrs[0] = entry;
	attrs[1] = info;
	attrs[2] = NULL;
	sp.attrs = attrs;

	/* Build a query string. */
	l = strlen("(objectclass=)") + strlen(class) + 1;

	sp.query = malloc(l);
	if (sp.query == NULL) {
		char *estr = strerror_r(errno, buf, sizeof(buf));
		logerr(MODPREFIX "malloc: %s", estr);
		return NSS_STATUS_UNAVAIL;
	}

	if (sprintf(sp.query, "(objectclass=%s)", class) >= l) {
		error(ap->logopt, MODPREFIX "error forming query string");
		free(sp.query);
		return NSS_STATUS_UNAVAIL;
	}

	/* Look around. */
	debug(ap->logopt,
	      MODPREFIX "searching for \"%s\" under \"%s\"", sp.query, ctxt->qdn);

	sp.cookie = NULL;
	sp.pageSize = 2000;
	sp.morePages = FALSE;
	sp.totalCount = 0;
	sp.result = NULL;

	do {
		rv = do_paged_query(&sp, ctxt);

		if (rv == LDAP_ADMINLIMIT_EXCEEDED ||
		    rv == LDAP_SIZELIMIT_EXCEEDED) {
			if (sp.result) {
				ldap_msgfree(sp.result);
				sp.result = NULL;
			}
			if (sp.cookie) {
				ber_bvfree(sp.cookie);
				sp.cookie = NULL;
			}
			sp.pageSize = sp.pageSize / 2;
			if (sp.pageSize < 5) {
				debug(ap->logopt, MODPREFIX
				      "result size too small");
				unbind_ldap_connection(ap->logopt, sp.ldap, ctxt);
				*result_ldap = rv;
				free(sp.query);
				return NSS_STATUS_UNAVAIL;
			}
			continue;
		}

		if (rv != LDAP_SUCCESS || !sp.result) {
			unbind_ldap_connection(ap->logopt, sp.ldap, ctxt);
			*result_ldap = rv;
			if (sp.result)
				ldap_msgfree(sp.result);
			if (sp.cookie)
				ber_bvfree(sp.cookie);
			free(sp.query);
			return NSS_STATUS_UNAVAIL;
		}

		rv = do_get_entries(&sp, source, ctxt);
		if (rv != LDAP_SUCCESS) {
			ldap_msgfree(sp.result);
			unbind_ldap_connection(ap->logopt, sp.ldap, ctxt);
			*result_ldap = rv;
			if (sp.cookie)
				ber_bvfree(sp.cookie);
			free(sp.query);
			return NSS_STATUS_NOTFOUND;
		}
		ldap_msgfree(sp.result);
		sp.result = NULL;
	} while (sp.morePages == TRUE);

	debug(ap->logopt, MODPREFIX "done updating map");

	unbind_ldap_connection(ap->logopt, sp.ldap, ctxt);

	source->age = age;
	if (sp.cookie)
		ber_bvfree(sp.cookie);
	free(sp.query);

	return NSS_STATUS_SUCCESS;
}