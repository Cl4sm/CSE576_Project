static int match_choice_label(struct openconnect_info *vpninfo,
			      struct oc_form_opt_select *select_opt,
			      char *label)
{
	int i, input_len, partial_matches = 0;
	char *match = NULL;

	input_len = strlen(label);
	if (input_len < 1)
		return -EINVAL;

	for (i = 0; i < select_opt->nr_choices; i++) {
		struct oc_choice *choice = select_opt->choices[i];

		if (!strncasecmp(label, choice->label, input_len)) {
			if (strlen(choice->label) == input_len) {
				select_opt->form.value = choice->name;
				return 0;
			} else {
				match = choice->name;
				partial_matches++;
			}
		}
	}

	if (partial_matches == 1) {
		select_opt->form.value = match;
		return 0;
	} else if (partial_matches > 1) {
		vpn_progress(vpninfo, PRG_ERR,
			     _("Auth choice \"%s\" matches multiple options\n"), label);
		return -EINVAL;
	} else {
		vpn_progress(vpninfo, PRG_ERR, _("Auth choice \"%s\" not available\n"), label);
		return -EINVAL;
	}
}