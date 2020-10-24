int
sasl_do_kinit_ext_cc(unsigned logopt, struct lookup_context *ctxt)
{
	krb5_principal def_princ;
	krb5_principal krb5_client_princ;
	krb5_error_code ret;
	char *cc_princ, *client_princ;

	if (ctxt->kinit_done)
		return 0;
	ctxt->kinit_done = 1;

	debug(logopt,
	      "using external credential cache for auth: client principal %s",
	      ctxt->client_princ ? ctxt->client_princ : default_client);

	ret = krb5_init_context(&ctxt->krb5ctxt);
	if (ret) {
		error(logopt, "krb5_init_context failed with %d", ret);
		return -1;
	}

	ret = krb5_cc_resolve(ctxt->krb5ctxt, ctxt->client_cc, &ctxt->krb5_ccache);
	if (ret) {
		error(logopt, "krb5_cc_resolve failed with error %d",
		      ret);
		krb5_cc_close(ctxt->krb5ctxt, ctxt->krb5_ccache);
		krb5_free_context(ctxt->krb5ctxt);
		return -1;
	}

	ret = krb5_cc_get_principal(ctxt->krb5ctxt, ctxt->krb5_ccache, &def_princ);
	if (ret) {
		error(logopt, "krb5_cc_get_principal failed with error %d", ret);
		krb5_cc_close(ctxt->krb5ctxt, ctxt->krb5_ccache);
		krb5_free_context(ctxt->krb5ctxt);
		return -1;
	}

	ret = krb5_unparse_name(ctxt->krb5ctxt, def_princ, &cc_princ);
	if (ret) {
		error(logopt, "krb5_unparse_name failed with error %d", ret);
		krb5_free_principal(ctxt->krb5ctxt, def_princ);
		krb5_cc_close(ctxt->krb5ctxt, ctxt->krb5_ccache);
		krb5_free_context(ctxt->krb5ctxt);
		return -1;
	}

	debug(logopt, "external credential cache default principal %s", cc_princ);

	/*
	 * If the principal isn't set in the config construct the default
	 * so we can check against the default principal of the external
	 * cred cache.
	 */
	if (ctxt->client_princ)
		client_princ = ctxt->client_princ;
	else {
		debug(logopt,
		      "calling krb5_sname_to_principal using defaults");

		ret = krb5_sname_to_principal(ctxt->krb5ctxt, NULL,
					default_client, KRB5_NT_SRV_HST, 
					&krb5_client_princ);
		if (ret) {
			error(logopt,
			      "krb5_sname_to_principal failed for "
			      "%s with error %d", default_client, ret);
			krb5_free_principal(ctxt->krb5ctxt, def_princ);
			krb5_cc_close(ctxt->krb5ctxt, ctxt->krb5_ccache);
			krb5_free_context(ctxt->krb5ctxt);
			return -1;
		}


		ret = krb5_unparse_name(ctxt->krb5ctxt,
					krb5_client_princ, &client_princ);
		if (ret) {
			debug(logopt,
			      "krb5_unparse_name failed with error %d",
			      ret);
			krb5_free_principal(ctxt->krb5ctxt, krb5_client_princ);
			krb5_free_principal(ctxt->krb5ctxt, def_princ);
			krb5_cc_close(ctxt->krb5ctxt, ctxt->krb5_ccache);
			krb5_free_context(ctxt->krb5ctxt);
			return -1;
		}

		debug(logopt,
		      "principal used for authentication: %s", client_princ);

		krb5_free_principal(ctxt->krb5ctxt, krb5_client_princ);
	}

	/*
	 * Check if the principal to be used matches the default principal in
	 * the external cred cache.
	 */
	if (strcmp(cc_princ, client_princ)) {
		error(logopt,
		      "configured client principal %s ",
		      ctxt->client_princ);
		error(logopt,
		      "external credential cache default principal %s",
		      cc_princ);
		error(logopt, 
		      "cannot use credential cache, external "
		      "default principal does not match configured "
		      "principal");
		if (!ctxt->client_princ)
			krb5_free_unparsed_name(ctxt->krb5ctxt, client_princ);
		krb5_free_unparsed_name(ctxt->krb5ctxt, cc_princ);
		krb5_free_principal(ctxt->krb5ctxt, def_princ);
		krb5_cc_close(ctxt->krb5ctxt, ctxt->krb5_ccache);
		krb5_free_context(ctxt->krb5ctxt);
		return -1;
	}

	if (!ctxt->client_princ)
		krb5_free_unparsed_name(ctxt->krb5ctxt, client_princ);
	krb5_free_unparsed_name(ctxt->krb5ctxt, cc_princ);
	krb5_free_principal(ctxt->krb5ctxt, def_princ);

	/* Set the environment variable to point to the external cred cache */
	if (setenv(krb5ccenv, ctxt->client_cc, 1) != 0) {
		error(logopt, "setenv failed with %d", errno);
		krb5_cc_close(ctxt->krb5ctxt, ctxt->krb5_ccache);
		krb5_free_context(ctxt->krb5ctxt);
		return -1;
	}
	ctxt->kinit_successful = 1;

	debug(logopt, "Kerberos authentication was successful!");

	return 0;
}