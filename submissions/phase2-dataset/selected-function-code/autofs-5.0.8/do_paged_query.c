static int do_paged_query(struct ldap_search_params *sp, struct lookup_context *ctxt)
{
	struct autofs_point *ap = sp->ap;
	LDAPControl *pageControl=NULL, *controls[2] = { NULL, NULL };
	LDAPControl **returnedControls = NULL;
	static char pagingCriticality = 'T';
	int rv, scope = LDAP_SCOPE_SUBTREE;

	if (sp->morePages == TRUE)
		goto do_paged;

	rv = ldap_search_s(sp->ldap, ctxt->qdn, scope, sp->query, sp->attrs, 0, &sp->result);
	if ((rv != LDAP_SUCCESS) || !sp->result) {
		/*
 		 * Check for Size Limit exceeded and force run through loop
		 * and requery using page control.
 		 */
		if (rv == LDAP_SIZELIMIT_EXCEEDED ||
		    rv == LDAP_ADMINLIMIT_EXCEEDED)
			sp->morePages = TRUE;
		else {
			debug(ap->logopt,
			      MODPREFIX "query failed for %s: %s",
			      sp->query, ldap_err2string(rv));
			return rv;
		}
	}
	return rv;

do_paged:
	/* we need to use page controls so requery LDAP */
	debug(ap->logopt, MODPREFIX "geting page of results");

	rv = ldap_create_page_control(sp->ldap, sp->pageSize, sp->cookie,
				      pagingCriticality, &pageControl);
	if (rv != LDAP_SUCCESS) {
		warn(ap->logopt, MODPREFIX "failed to create page control");
		return rv;
	}

	/* Insert the control into a list to be passed to the search. */
	controls[0] = pageControl;

	/* Search for entries in the directory using the parmeters. */
	rv = ldap_search_ext_s(sp->ldap,
			       ctxt->qdn, scope, sp->query, sp->attrs,
			       0, controls, NULL, NULL, 0, &sp->result);
	if ((rv != LDAP_SUCCESS) && (rv != LDAP_PARTIAL_RESULTS)) {
		ldap_control_free(pageControl);
		if (rv != LDAP_ADMINLIMIT_EXCEEDED)
			debug(ap->logopt,
			      MODPREFIX "query failed for %s: %s",
			      sp->query, ldap_err2string(rv));
		return rv;
	}

	/* Parse the results to retrieve the contols being returned. */
	rv = ldap_parse_result(sp->ldap, sp->result,
			       NULL, NULL, NULL, NULL,
			       &returnedControls, FALSE);
	if (sp->cookie != NULL) {
		ber_bvfree(sp->cookie);
		sp->cookie = NULL;
	}

	if (rv != LDAP_SUCCESS) {
		debug(ap->logopt,
		      MODPREFIX "ldap_parse_result failed with %d", rv);
		goto out_free;
	}

	/*
	 * Parse the page control returned to get the cookie and
	 * determine whether there are more pages.
	 */
	rv = ldap_parse_page_control(sp->ldap,
				     returnedControls, &sp->totalCount,
				     &sp->cookie);
	if (sp->cookie && sp->cookie->bv_val &&
	    (strlen(sp->cookie->bv_val) || sp->cookie->bv_len))
		sp->morePages = TRUE;
	else
		sp->morePages = FALSE;

	/* Cleanup the controls used. */
	if (returnedControls)
		ldap_controls_free(returnedControls);

out_free:
	ldap_control_free(pageControl);
	return rv;
}