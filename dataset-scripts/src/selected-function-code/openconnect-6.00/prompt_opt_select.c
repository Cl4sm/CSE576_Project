static int prompt_opt_select(struct openconnect_info *vpninfo,
			     struct oc_form_opt_select *select_opt,
			     char **saved_response)
{
	int i;
	char *response;

	if (!select_opt->nr_choices)
		return -EINVAL;

retry:
	fprintf(stderr, "%s [", select_opt->form.label);
	for (i = 0; i < select_opt->nr_choices; i++) {
		struct oc_choice *choice = select_opt->choices[i];
		if (i)
			fprintf(stderr, "|");

		fprintf(stderr, "%s", choice->label);
	}
	fprintf(stderr, "]:");

	if (select_opt->nr_choices == 1) {
		response = strdup(select_opt->choices[0]->label);
		fprintf(stderr, "%s\n", response);
	} else
		response = prompt_for_input("", vpninfo, 0);

	if (!response)
		return -EINVAL;

	if (match_choice_label(vpninfo, select_opt, response) < 0) {
		free(response);
		goto retry;
	}

	if (saved_response)
		*saved_response = response;
	else
		free(response);

	return 0;
}