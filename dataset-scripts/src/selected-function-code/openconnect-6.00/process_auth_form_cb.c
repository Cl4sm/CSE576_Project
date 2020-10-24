static int process_auth_form_cb(void *_vpninfo,
				struct oc_auth_form *form)
{
	struct openconnect_info *vpninfo = _vpninfo;
	struct oc_form_opt *opt;
	int empty = 1;

	if (form->banner && verbose > PRG_ERR)
		fprintf(stderr, "%s\n", form->banner);

	if (form->error)
		fprintf(stderr, "%s\n", form->error);

	if (form->message && verbose > PRG_ERR)
		fprintf(stderr, "%s\n", form->message);

	/* Special handling for GROUP: field if present, as different group
	   selections can make other fields disappear/reappear */
	if (form->authgroup_opt) {
		if (!authgroup ||
		    match_choice_label(vpninfo, form->authgroup_opt, authgroup) != 0) {
			if (prompt_opt_select(vpninfo, form->authgroup_opt, &authgroup) < 0)
				goto err;
		}
		if (!authgroup_set) {
			authgroup_set = 1;
			return OC_FORM_RESULT_NEWGROUP;
		}
	}

	for (opt = form->opts; opt; opt = opt->next) {

		if (opt->flags & OC_FORM_OPT_IGNORE)
			continue;

		/* I haven't actually seen a non-authgroup dropdown in the wild, but
		   the Cisco clients do support them */
		if (opt->type == OC_FORM_OPT_SELECT) {
			struct oc_form_opt_select *select_opt = (void *)opt;

			if (select_opt == form->authgroup_opt)
				continue;
			if (prompt_opt_select(vpninfo, select_opt, NULL) < 0)
				goto err;
			empty = 0;

		} else if (opt->type == OC_FORM_OPT_TEXT) {
			if (username &&
			    !strcmp(opt->name, "username")) {
				opt->value = username;
				username = NULL;
			} else {
				opt->value = prompt_for_input(opt->label, vpninfo, 0);
			}

			if (!opt->value)
				goto err;
			empty = 0;

		} else if (opt->type == OC_FORM_OPT_PASSWORD) {
			if (password &&
			    !strcmp(opt->name, "password")) {
				opt->value = password;
				password = NULL;
			} else {
				opt->value = prompt_for_input(opt->label, vpninfo, 1);
			}

			if (!opt->value)
				goto err;
			empty = 0;
		}
	}

	/* prevent infinite loops if the authgroup requires certificate auth only */
	if (last_form_empty && empty)
		return OC_FORM_RESULT_CANCELLED;
	last_form_empty = empty;

	return OC_FORM_RESULT_OK;

 err:
	return OC_FORM_RESULT_ERR;
}