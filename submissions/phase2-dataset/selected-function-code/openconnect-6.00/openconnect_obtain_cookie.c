int openconnect_obtain_cookie(struct openconnect_info *vpninfo)
{
	struct oc_vpn_option *opt;
	char *form_buf = NULL;
	struct oc_auth_form *form = NULL;
	int result, buflen, tries;
	char request_body[2048];
	const char *request_body_type = "application/x-www-form-urlencoded";
	const char *method = "POST";
	char *orig_host = NULL, *orig_path = NULL, *form_path = NULL;
	int orig_port = 0;
	int cert_rq, cert_sent = !vpninfo->cert;

	/* Step 1: Unlock software token (if applicable) */
	if (vpninfo->token_mode == OC_TOKEN_MODE_STOKEN) {
		result = prepare_stoken(vpninfo);
		if (result)
			return result;
	}

	if (!vpninfo->xmlpost)
		goto no_xmlpost;

	/*
	 * Step 2: Probe for XML POST compatibility
	 *
	 * This can get stuck in a redirect loop, so give up after any of:
	 *
	 * a) HTTP error (e.g. 400 Bad Request)
	 * b) Same-host redirect (e.g. Location: /foo/bar)
	 * c) Three redirects without seeing a plausible login form
	 */
newgroup:
	result = xmlpost_initial_req(vpninfo, request_body, sizeof(request_body), 0);
	if (result < 0)
		return result;

	orig_host = strdup(vpninfo->hostname);
	orig_path = vpninfo->urlpath ? strdup(vpninfo->urlpath) : NULL;
	orig_port = vpninfo->port;

	for (tries = 0; ; tries++) {
		if (tries == 3) {
		fail:
			if (vpninfo->xmlpost) {
			no_xmlpost:
				/* Try without XML POST this time... */
				tries = 0;
				vpninfo->xmlpost = 0;
				request_body_type = NULL;
				request_body[0] = 0;
				method = "GET";
				if (orig_host) {
					openconnect_set_hostname(vpninfo, orig_host);
					orig_host = NULL;
					free(vpninfo->urlpath);
					vpninfo->urlpath = orig_path;
					orig_path = NULL;
					vpninfo->port = orig_port;
				}
				openconnect_close_https(vpninfo, 0);
			} else {
				free(orig_host);
				return -EIO;
			}
		}

		buflen = do_https_request(vpninfo, method, request_body_type, request_body,
					  &form_buf, 0);
		if (vpninfo->got_cancel_cmd)
			return 1;
		if (buflen == -EINVAL)
			goto fail;
		if (buflen < 0) {
			free(orig_host);
			return buflen;
		}

		/* Some ASAs forget to send the TLS cert request on the initial connection.
		 * If we have a client cert, disable HTTP keepalive until we get a real
		 * login form (not a redirect). */
		if (!cert_sent)
			openconnect_close_https(vpninfo, 0);

		/* XML POST does not allow local redirects, but GET does. */
		if (vpninfo->xmlpost &&
		    vpninfo->redirect_type == REDIR_TYPE_LOCAL)
			goto fail;
		else if (vpninfo->redirect_type != REDIR_TYPE_NONE)
			continue;

		result = parse_xml_response(vpninfo, form_buf, &form, &cert_rq);
		if (result < 0)
			goto fail;

		if (cert_rq) {
			int cert_failed = 0;

			free_auth_form(form);
			form = NULL;

			if (!cert_sent && vpninfo->cert) {
				/* Try again on a fresh connection. */
				cert_sent = 1;
			} else if (cert_sent && vpninfo->cert) {
				/* Try again with <client-cert-fail/> in the request */
				vpn_progress(vpninfo, PRG_ERR,
					     _("Server requested SSL client certificate after one was provided\n"));
				cert_failed = 1;
			} else {
				vpn_progress(vpninfo, PRG_INFO,
					     _("Server requested SSL client certificate; none was configured\n"));
				cert_failed = 1;
			}
			result = xmlpost_initial_req(vpninfo, request_body, sizeof(request_body), cert_failed);
			if (result < 0)
				goto fail;
			continue;
		}
		if (form && form->action) {
			vpninfo->redirect_url = strdup(form->action);
			handle_redirect(vpninfo);
		}
		break;
	}
	if (vpninfo->xmlpost)
		vpn_progress(vpninfo, PRG_INFO, _("XML POST enabled\n"));

	free (orig_host);
	free (orig_path);

	/* Step 4: Run the CSD trojan, if applicable */
	if (vpninfo->csd_starturl && vpninfo->csd_waiturl) {
		buflen = 0;

		if (vpninfo->urlpath) {
			form_path = strdup(vpninfo->urlpath);
			if (!form_path) {
				result = -ENOMEM;
				goto out;
			}
		}

		/* fetch the CSD program, if available */
		if (vpninfo->csd_stuburl) {
			vpninfo->redirect_url = vpninfo->csd_stuburl;
			vpninfo->csd_stuburl = NULL;
			handle_redirect(vpninfo);

			buflen = do_https_request(vpninfo, "GET", NULL, NULL, &form_buf, 0);
			if (buflen <= 0) {
				result = -EINVAL;
				goto out;
			}
		}

		/* This is the CSD stub script, which we now need to run */
		result = run_csd_script(vpninfo, form_buf, buflen);
		if (result)
			goto out;

		/* vpninfo->urlpath now points to the wait page */
		while (1) {
			result = do_https_request(vpninfo, "GET", NULL, NULL, &form_buf, 0);
			if (result <= 0)
				break;

			result = check_response_type(vpninfo, form_buf);
			if (result <= 0)
				break;

			vpn_progress(vpninfo, PRG_INFO,
				     _("Refreshing %s after 1 second...\n"),
				     vpninfo->urlpath);
			sleep(1);
		}
		if (result < 0)
			goto out;

		/* refresh the form page, to see if we're authorized now */
		free(vpninfo->urlpath);
		vpninfo->urlpath = form_path;
		form_path = NULL;

		result = do_https_request(vpninfo,
					  vpninfo->xmlpost ? "POST" : "GET",
					  request_body_type, request_body, &form_buf, 1);
		if (result < 0)
			goto out;

		result = parse_xml_response(vpninfo, form_buf, &form, NULL);
		if (result < 0)
			goto out;
	}

	/* Step 5: Ask the user to fill in the auth form; repeat as necessary */
	while (1) {
		request_body[0] = 0;
		result = handle_auth_form(vpninfo, form, request_body, sizeof(request_body),
					  &method, &request_body_type);
		if (result < 0 || result == OC_FORM_RESULT_CANCELLED)
			goto out;
		if (result == OC_FORM_RESULT_LOGGEDIN)
			break;
		if (result == OC_FORM_RESULT_NEWGROUP) {
			free(form_buf);
			form_buf = NULL;
			free_auth_form(form);
			form = NULL;
			goto newgroup;
		}

		result = do_https_request(vpninfo, method, request_body_type, request_body,
					  &form_buf, 1);
		if (result < 0)
			goto out;

		result = parse_xml_response(vpninfo, form_buf, &form, NULL);
		if (result < 0)
			goto out;
		if (form->action) {
			vpninfo->redirect_url = strdup(form->action);
			handle_redirect(vpninfo);
		}
	}

	/* A return value of 2 means the XML form indicated
	   success. We _should_ have a cookie... */

	for (opt = vpninfo->cookies; opt; opt = opt->next) {

		if (!strcmp(opt->option, "webvpn")) {
			free(vpninfo->cookie);
			vpninfo->cookie = strdup(opt->value);
		} else if (vpninfo->write_new_config && !strcmp(opt->option, "webvpnc")) {
			char *tok = opt->value;
			char *bu = NULL, *fu = NULL, *sha = NULL;

			do {
				if (tok != opt->value)
					*(tok++) = 0;

				if (!strncmp(tok, "bu:", 3))
					bu = tok + 3;
				else if (!strncmp(tok, "fu:", 3))
					fu = tok + 3;
				else if (!strncmp(tok, "fh:", 3))
					sha = tok + 3;
			} while ((tok = strchr(tok, '&')));

			if (bu && fu && sha) {
				if (asprintf(&vpninfo->profile_url, "%s%s", bu, fu) == -1) {
					result = -ENOMEM;
					goto out;
				}
				vpninfo->profile_sha1 = strdup(sha);
			}
		}
	}
	result = 0;

	fetch_config(vpninfo);

out:
	free(form_path);
	free(form_buf);
	free_auth_form(form);

	if (vpninfo->csd_scriptname) {
		unlink(vpninfo->csd_scriptname);
		free(vpninfo->csd_scriptname);
		vpninfo->csd_scriptname = NULL;
	}

	return result;
}