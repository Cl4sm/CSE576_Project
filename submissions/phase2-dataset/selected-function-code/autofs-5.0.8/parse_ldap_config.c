int parse_ldap_config(unsigned logopt, struct lookup_context *ctxt)
{
	int          ret = 0, fallback = 0;
	unsigned int auth_required = LDAP_AUTH_NOTREQUIRED;
	unsigned int tls_required = 0, use_tls = 0;
	struct stat  st;
	xmlDocPtr    doc = NULL;
	xmlNodePtr   root = NULL;
	char         *authrequired, *auth_conf, *authtype;
	char         *user = NULL, *secret = NULL;
	char         *extern_cert = NULL, *extern_key = NULL;
	char         *client_princ = NULL, *client_cc = NULL;
	char	     *usetls, *tlsrequired;

	authtype = user = secret = NULL;

	auth_conf = (char *) defaults_get_auth_conf_file();
	if (!auth_conf) {
		error(logopt,
		      MODPREFIX "failed to get auth config file name.");
		return 0;
	}

	/*
	 *  Here we check that the config file exists, and that we have
	 *  permission to read it.  The XML library does not specify why a
	 *  parse happens to fail, so we have to do all of this checking
	 *  beforehand.
	 */
	memset(&st, 0, sizeof(st));
	if (stat(auth_conf, &st) == -1 || st.st_size == 0) {
		/* Auth config doesn't exist so disable TLS and auth */
		if (errno == ENOENT) {
			ctxt->auth_conf = auth_conf;
			ctxt->use_tls = LDAP_TLS_DONT_USE;
			ctxt->tls_required = LDAP_TLS_DONT_USE;
			ctxt->auth_required = LDAP_AUTH_NOTREQUIRED;
			ctxt->sasl_mech = NULL;
			ctxt->user = NULL;
			ctxt->secret = NULL;
			ctxt->client_princ = NULL;
			return 0;
		}
		error(logopt,
		      MODPREFIX "stat(2) failed with error %s.",
		      strerror(errno));
		return 0;
	}

	if (!S_ISREG(st.st_mode) ||
	    st.st_uid != 0 || st.st_gid != 0 ||
	    (st.st_mode & 0x01ff) != 0600) {
		error(logopt, MODPREFIX
		      "Configuration file %s exists, but is not usable. "
		      "Please make sure that it is owned by root, group "
		      "is root, and the mode is 0600.",
		      auth_conf);
		return -1;
	}

	doc = xmlParseFile(auth_conf);
	if (!doc) {
		error(logopt, MODPREFIX
		     "xmlParseFile failed for %s.", auth_conf);
		goto out;
	}

	root = xmlDocGetRootElement(doc);
	if (!root) {
		debug(logopt, MODPREFIX
		      "empty xml document (%s).", auth_conf);
		fallback = 1;
		goto out;
	}

	if (xmlStrcmp(root->name, (const xmlChar *)"autofs_ldap_sasl_conf")) {
		error(logopt, MODPREFIX
		      "The root node of the XML document %s is not "
		      "autofs_ldap_sasl_conf.", auth_conf);
		goto out;
	}

	ret = get_property(logopt, root, "usetls", &usetls);
	if (ret != 0) {
		error(logopt,
		      MODPREFIX
		      "Failed read the usetls property from "
		      "the configuration file %s.", auth_conf);
		goto out;
	}

	if (!usetls || ctxt->port == LDAPS_PORT)
		use_tls = LDAP_TLS_DONT_USE;
	else {
		if (!strcasecmp(usetls, "yes"))
			use_tls = LDAP_TLS_INIT;
		else if (!strcasecmp(usetls, "no"))
			use_tls = LDAP_TLS_DONT_USE;
		else {
			error(logopt,
			      MODPREFIX
			      "The usetls property must have value "
			      "\"yes\" or \"no\".");
			ret = -1;
			goto out;
		}
		free(usetls);
	}

	ret = get_property(logopt, root, "tlsrequired", &tlsrequired);
	if (ret != 0) {
		error(logopt,
		      MODPREFIX
		      "Failed read the tlsrequired property from "
		      "the configuration file %s.", auth_conf);
		goto out;
	}

	if (!tlsrequired)
		tls_required = LDAP_TLS_DONT_USE;
	else {
		if (!strcasecmp(tlsrequired, "yes"))
			tls_required = LDAP_TLS_REQUIRED;
		else if (!strcasecmp(tlsrequired, "no"))
			tls_required = LDAP_TLS_DONT_USE;
		else {
			error(logopt,
			      MODPREFIX
			      "The tlsrequired property must have value "
			      "\"yes\" or \"no\".");
			ret = -1;
			goto out;
		}
		free(tlsrequired);
	}

	ret = get_property(logopt, root, "authrequired", &authrequired);
	if (ret != 0) {
		error(logopt,
		      MODPREFIX
		      "Failed read the authrequired property from "
		      "the configuration file %s.", auth_conf);
		goto out;
	}

	if (!authrequired)
		auth_required = LDAP_AUTH_NOTREQUIRED;
	else {
		if (!strcasecmp(authrequired, "yes"))
			auth_required = LDAP_AUTH_REQUIRED;
		else if (!strcasecmp(authrequired, "no"))
			auth_required = LDAP_AUTH_NOTREQUIRED;
		else if (!strcasecmp(authrequired, "autodetect"))
			auth_required = LDAP_AUTH_AUTODETECT;
		else if (!strcasecmp(authrequired, "simple"))
			auth_required = LDAP_AUTH_USESIMPLE;
		else {
			error(logopt,
			      MODPREFIX
			      "The authrequired property must have value "
			      "\"yes\", \"no\", \"autodetect\", or \"simple\".");
			ret = -1;
			goto out;
		}
		free(authrequired);
	}

	ret = get_property(logopt, root, "authtype", &authtype);
	if (ret != 0) {
		error(logopt,
		      MODPREFIX
		      "Failed read the authtype property from the "
		      "configuration file %s.", auth_conf);
		goto out;
	}

	if (auth_required == LDAP_AUTH_USESIMPLE ||
	   (authtype && authtype_requires_creds(authtype))) {
		char *s1 = NULL, *s2 = NULL;
		ret = get_property(logopt, root, "user",  &user);
		ret |= get_property(logopt, root, "secret", &s1);
		ret |= get_property(logopt, root, "encoded_secret", &s2);
		if (ret != 0 || (!user || (!s1 && !s2))) {
auth_fail:
			error(logopt,
			      MODPREFIX
			      "%s authentication type requires a username "
			      "and a secret.  Please fix your configuration "
			      "in %s.", authtype, auth_conf);
			free(authtype);
			if (user)
				free(user);
			if (s1)
				free(s1);
			if (s2)
				free(s2);

			ret = -1;
			goto out;
		}
		if (!s2)
			secret = s1;
		else {
			char dec_buf[120];
			int dec_len = base64_decode(s2, dec_buf, 119);
			if (dec_len <= 0)
				goto auth_fail;
			secret = strdup(dec_buf);
			if (!secret)
				goto auth_fail;
			if (s1)
				free(s1);
			if (s2)
				free(s2);
		}
	} else if (auth_required == LDAP_AUTH_REQUIRED &&
		  (authtype && !strncmp(authtype, "EXTERNAL", 8))) {
#ifdef WITH_SASL
		ret = get_property(logopt, root, "external_cert",  &extern_cert);
		ret |= get_property(logopt, root, "external_key",  &extern_key);
		/*
		 * For EXTERNAL auth to function we need a client certificate
		 * and and certificate key. The ca certificate used to verify
		 * the server certificate must also be set correctly in the
		 * global configuration as the connection must be encrypted
		 * and the server and client certificates must have been
		 * verified for the EXTERNAL method to be offerred by the
		 * server. If the cert and key have not been set in the autofs
		 * configuration they must be set in the ldap rc file.
		 */
		if (ret != 0 || !extern_cert || !extern_key) {
			if (extern_cert)
				free(extern_cert);
			if (extern_key)
				free(extern_key);
		}
#endif
	}

	/*
	 * We allow the admin to specify the principal to use for the
	 * client.  The default is "autofsclient/hostname@REALM".
	 */
	(void)get_property(logopt, root, "clientprinc", &client_princ);
	(void)get_property(logopt, root, "credentialcache", &client_cc);

	ctxt->auth_conf = auth_conf;
	ctxt->use_tls = use_tls;
	ctxt->tls_required = tls_required;
	ctxt->auth_required = auth_required;
	ctxt->sasl_mech = authtype;
	if (!authtype && (auth_required & LDAP_AUTH_REQUIRED))
		ctxt->auth_required |= LDAP_AUTH_AUTODETECT;
	ctxt->user = user;
	ctxt->secret = secret;
	ctxt->client_princ = client_princ;
	ctxt->client_cc = client_cc;
#ifdef WITH_SASL
	ctxt->extern_cert = extern_cert;
	ctxt->extern_key = extern_key;
#endif

	debug(logopt, MODPREFIX
	      "ldap authentication configured with the following options:");
	debug(logopt, MODPREFIX
	      "use_tls: %u, "
	      "tls_required: %u, "
	      "auth_required: %u, "
	      "sasl_mech: %s",
	      use_tls, tls_required, auth_required, authtype);
	if (authtype && !strncmp(authtype, "EXTERNAL", 8)) {
		debug(logopt, MODPREFIX "external cert: %s",
		      extern_cert ? extern_cert : "ldap default");
		debug(logopt, MODPREFIX "external key: %s ",
		      extern_key ? extern_key : "ldap default");
	} else {
		debug(logopt, MODPREFIX
		      "user: %s, "
		      "secret: %s, "
		      "client principal: %s "
		      "credential cache: %s",
		      user, secret ? "specified" : "unspecified",
		      client_princ, client_cc);
	}
out:
	xmlFreeDoc(doc);

	if (fallback)
		return 0;

	return ret;
}