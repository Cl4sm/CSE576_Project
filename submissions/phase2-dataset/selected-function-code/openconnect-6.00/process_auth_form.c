int process_auth_form(struct openconnect_info *vpninfo, struct oc_auth_form *form)
{
	int ret;
	struct oc_form_opt_select *grp = form->authgroup_opt;
	struct oc_choice *auth_choice;
	struct oc_form_opt *opt;

	if (!vpninfo->process_auth_form) {
		vpn_progress(vpninfo, PRG_ERR, _("No form handler; cannot authenticate.\n"));
		return OC_FORM_RESULT_ERR;
	}

retry:
	auth_choice = NULL;
	if (grp && grp->nr_choices && !vpninfo->xmlpost) {
		if (vpninfo->authgroup) {
			/* For non-XML-POST, the server doesn't tell us which group is selected */
			int i;
			for (i = 0; i < grp->nr_choices; i++)
				if (!strcmp(grp->choices[i]->name, vpninfo->authgroup))
					form->authgroup_selection = i;
		}
		auth_choice = grp->choices[form->authgroup_selection];
	}

	for (opt = form->opts; opt; opt = opt->next) {
		int second_auth = opt->flags & OC_FORM_OPT_SECOND_AUTH;
		opt->flags &= ~OC_FORM_OPT_IGNORE;

		if (!auth_choice ||
		    (opt->type != OC_FORM_OPT_TEXT && opt->type != OC_FORM_OPT_PASSWORD))
			continue;

		if (auth_choice->noaaa ||
		    (!auth_choice->second_auth && second_auth))
			opt->flags |= OC_FORM_OPT_IGNORE;
		else if (!strcmp(opt->name, "secondary_username") && second_auth) {
			if (auth_choice->secondary_username) {
				free(opt->value);
				opt->value = strdup(auth_choice->secondary_username);
			}
			if (!auth_choice->secondary_username_editable)
				opt->flags |= OC_FORM_OPT_IGNORE;
		}
	}

	ret = vpninfo->process_auth_form(vpninfo->cbdata, form);

	if (ret == OC_FORM_RESULT_NEWGROUP &&
	    form->authgroup_opt &&
	    form->authgroup_opt->form.value) {
		free(vpninfo->authgroup);
		vpninfo->authgroup = strdup(form->authgroup_opt->form.value);

		if (!vpninfo->xmlpost)
			goto retry;
	}

	if (ret == OC_FORM_RESULT_CANCELLED || ret < 0)
		nuke_opt_values(form->opts);

	return ret;
}