int handle_auth_form(struct openconnect_info *vpninfo, struct oc_auth_form *form,
		     char *request_body, int req_len, const char **method,
		     const char **request_body_type)
{
	int ret;
	struct oc_vpn_option *opt, *next;

	if (!strcmp(form->auth_id, "success"))
		return OC_FORM_RESULT_LOGGEDIN;

	if (vpninfo->nopasswd) {
		vpn_progress(vpninfo, PRG_ERR,
			     _("Asked for password but '--no-passwd' set\n"));
		return -EPERM;
	}

	if (vpninfo->csd_token && vpninfo->csd_ticket && vpninfo->csd_starturl && vpninfo->csd_waiturl) {
		/* AB: remove all cookies */
		for (opt = vpninfo->cookies; opt; opt = next) {
			next = opt->next;

			free(opt->option);
			free(opt->value);
			free(opt);
		}
		vpninfo->cookies = NULL;
		return OC_FORM_RESULT_OK;
	}
	if (!form->opts) {
		if (form->message)
			vpn_progress(vpninfo, PRG_INFO, "%s\n", form->message);
		if (form->error)
			vpn_progress(vpninfo, PRG_ERR, "%s\n", form->error);
		return -EPERM;
	}

	ret = process_auth_form(vpninfo, form);
	if (ret)
		return ret;

	/* tokencode generation is deferred until after username prompts and CSD */
	ret = do_gen_tokencode(vpninfo, form);
	if (ret)
		return ret;

	ret = vpninfo->xmlpost ?
	      xmlpost_append_form_opts(vpninfo, form, request_body, req_len) :
	      append_form_opts(vpninfo, form, request_body, req_len);
	if (!ret) {
		*method = "POST";
		*request_body_type = "application/x-www-form-urlencoded";
	}
	return ret;
}