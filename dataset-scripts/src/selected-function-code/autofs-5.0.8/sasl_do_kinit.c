int
sasl_do_kinit(unsigned logopt, struct lookup_context *ctxt)
{
	krb5_error_code ret;
	krb5_principal tgs_princ, krb5_client_princ;
	krb5_creds my_creds;
	char *tgs_name;
	const char *realm_name;
	int status, realm_length;

	if (ctxt->kinit_done)
		return 0;
	ctxt->kinit_done = 1;

	debug(logopt,
	      "initializing kerberos ticket: client principal %s",
	      ctxt->client_princ ? ctxt->client_princ : default_client);

	ret = krb5_init_context(&ctxt->krb5ctxt);
	if (ret) {
		error(logopt, "krb5_init_context failed with %d", ret);
		return -1;
	}

	ret = krb5_cc_resolve(ctxt->krb5ctxt, krb5ccval, &ctxt->krb5_ccache);
	if (ret) {
		error(logopt, "krb5_cc_resolve failed with error %d",
		      ret);
		krb5_free_context(ctxt->krb5ctxt);
		return -1;
	}

	if (ctxt->client_princ) {
		debug(logopt,
		      "calling krb5_parse_name on client principal %s",
		      ctxt->client_princ);

		ret = krb5_parse_name(ctxt->krb5ctxt, ctxt->client_princ,
				      &krb5_client_princ);
		if (ret) {
			error(logopt,
			      "krb5_parse_name failed for "
			      "specified client principal %s",
			      ctxt->client_princ);
			goto out_cleanup_cc;
		}
	} else {
		char *tmp_name = NULL;

		debug(logopt,
		      "calling krb5_sname_to_principal using defaults");

		ret = krb5_sname_to_principal(ctxt->krb5ctxt, NULL,
					default_client, KRB5_NT_SRV_HST, 
					&krb5_client_princ);
		if (ret) {
			error(logopt,
			      "krb5_sname_to_principal failed for "
			      "%s with error %d", default_client, ret);
			goto out_cleanup_cc;
		}


		ret = krb5_unparse_name(ctxt->krb5ctxt,
					krb5_client_princ, &tmp_name);
		if (ret) {
			debug(logopt,
			      "krb5_unparse_name failed with error %d",
			      ret);
			goto out_cleanup_client_princ;
		}

		debug(logopt,
		      "principal used for authentication: %s", tmp_name);

		krb5_free_unparsed_name(ctxt->krb5ctxt, tmp_name);
	}

	/* setup a principal for the ticket granting service */
	_krb5_princ_realm(ctxt->krb5ctxt, krb5_client_princ, &realm_name, &realm_length);
	ret = krb5_build_principal_ext(ctxt->krb5ctxt, &tgs_princ,
		realm_length, realm_name,
		strlen(KRB5_TGS_NAME), KRB5_TGS_NAME,
		realm_length, realm_name,
		0);
	if (ret) {
		error(logopt,
		      "krb5_build_principal failed with error %d", ret);
		goto out_cleanup_client_princ;
	}

	ret = krb5_unparse_name(ctxt->krb5ctxt, tgs_princ, &tgs_name);
	if (ret) {
		error(logopt, "krb5_unparse_name failed with error %d",
		      ret);
		goto out_cleanup_client_princ;
	}

	debug(logopt, "Using tgs name %s", tgs_name);

	memset(&my_creds, 0, sizeof(my_creds));
	ret = krb5_get_init_creds_keytab(ctxt->krb5ctxt, &my_creds,
					 krb5_client_princ,
					 NULL /*keytab*/,
					 0 /* relative start time */,
					 tgs_name, NULL);
	if (ret) {
		error(logopt,
		      "krb5_get_init_creds_keytab failed with error %d",
		      ret);
		goto out_cleanup_unparse;
	}

	status = pthread_mutex_lock(&krb5cc_mutex);
	if (status)
		fatal(status);

	if (krb5cc_in_use++ == 0)
		/* tell the cache what the default principal is */
		ret = krb5_cc_initialize(ctxt->krb5ctxt,
				 ctxt->krb5_ccache, krb5_client_princ);

	status = pthread_mutex_unlock(&krb5cc_mutex);
	if (status)
		fatal(status);

	if (ret) {
		error(logopt,
		      "krb5_cc_initialize failed with error %d", ret);
		goto out_cleanup_creds;
	}

	/* and store credentials for that principal */
	ret = krb5_cc_store_cred(ctxt->krb5ctxt, ctxt->krb5_ccache, &my_creds);
	if (ret) {
		error(logopt,
		      "krb5_cc_store_cred failed with error %d", ret);
		goto out_cleanup_creds;
	}

	/* finally, set the environment variable to point to our
	 * credentials cache */
	if (setenv(krb5ccenv, krb5ccval, 1) != 0) {
		error(logopt, "setenv failed with %d", errno);
		goto out_cleanup_creds;
	}
	ctxt->kinit_successful = 1;

	debug(logopt, "Kerberos authentication was successful!");

	krb5_free_unparsed_name(ctxt->krb5ctxt, tgs_name);
	krb5_free_cred_contents(ctxt->krb5ctxt, &my_creds);
	krb5_free_principal(ctxt->krb5ctxt, tgs_princ);
	krb5_free_principal(ctxt->krb5ctxt, krb5_client_princ);

	return 0;

out_cleanup_creds:
	krb5cc_in_use--;
	krb5_free_cred_contents(ctxt->krb5ctxt, &my_creds);
out_cleanup_unparse:
	krb5_free_principal(ctxt->krb5ctxt, tgs_princ);
	krb5_free_unparsed_name(ctxt->krb5ctxt, tgs_name);
out_cleanup_client_princ:
	krb5_free_principal(ctxt->krb5ctxt, krb5_client_princ);
out_cleanup_cc:
	status = pthread_mutex_lock(&krb5cc_mutex);
	if (status)
		fatal(status);

	if (krb5cc_in_use)
		ret = krb5_cc_close(ctxt->krb5ctxt, ctxt->krb5_ccache);
	else
		ret = krb5_cc_destroy(ctxt->krb5ctxt, ctxt->krb5_ccache);
	if (ret)
		warn(logopt,
		     "krb5_cc_destroy failed with non-fatal error %d", ret);

	status = pthread_mutex_unlock(&krb5cc_mutex);
	if (status)
		fatal(status);

	krb5_free_context(ctxt->krb5ctxt);

	return -1;
}