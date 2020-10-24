static int parse_auth_choice(struct openconnect_info *vpninfo, struct oc_auth_form *form,
			     xmlNode *xml_node)
{
	struct oc_form_opt_select *opt;
	xmlNode *opt_node;
	int max_choices = 0, selection = 0;

	opt = calloc(1, sizeof(*opt));
	if (!opt)
		return -ENOMEM;

	opt->form.type = OC_FORM_OPT_SELECT;
	opt->form.name = (char *)xmlGetProp(xml_node, (unsigned char *)"name");
	opt->form.label = (char *)xmlGetProp(xml_node, (unsigned char *)"label");

	if (!opt->form.name) {
		vpn_progress(vpninfo, PRG_ERR, _("Form choice has no name\n"));
		free_opt((struct oc_form_opt *)opt);
		return -EINVAL;
	}

	for (opt_node = xml_node->children; opt_node; opt_node = opt_node->next)
		max_choices++;

	opt->choices = calloc(1, max_choices * sizeof(struct oc_choice *));
	if (!opt->choices) {
		free_opt((struct oc_form_opt *)opt);
		return -ENOMEM;
	}

	for (xml_node = xml_node->children; xml_node; xml_node = xml_node->next) {
		char *form_id;
		struct oc_choice *choice;

		if (xml_node->type != XML_ELEMENT_NODE)
			continue;

		if (strcmp((char *)xml_node->name, "option"))
			continue;

		form_id = (char *)xmlGetProp(xml_node, (unsigned char *)"value");
		if (!form_id)
			form_id = (char *)xmlNodeGetContent(xml_node);
		if (!form_id)
			continue;

		choice = calloc(1, sizeof(*choice));
		if (!choice) {
			free_opt((struct oc_form_opt *)opt);
			return -ENOMEM;
		}

		choice->name = form_id;
		choice->label = (char *)xmlNodeGetContent(xml_node);
		choice->auth_type = (char *)xmlGetProp(xml_node, (unsigned char *)"auth-type");
		choice->override_name = (char *)xmlGetProp(xml_node, (unsigned char *)"override-name");
		choice->override_label = (char *)xmlGetProp(xml_node, (unsigned char *)"override-label");

		choice->second_auth = prop_equals(xml_node, "second-auth", "1");
		choice->secondary_username = (char *)xmlGetProp(xml_node,
			(unsigned char *)"secondary_username");
		choice->secondary_username_editable = prop_equals(xml_node,
			"secondary_username_editable", "true");
		choice->noaaa = prop_equals(xml_node, "noaaa", "1");

		if (prop_equals(xml_node, "selected", "true"))
			selection = opt->nr_choices;

		opt->choices[opt->nr_choices++] = choice;
	}

	if (!strcmp(opt->form.name, "group_list")) {
		form->authgroup_opt = opt;
		form->authgroup_selection = selection;
	}

	/* We link the choice _first_ so it's at the top of what we present
	   to the user */
	opt->form.next = form->opts;
	form->opts = &opt->form;
	return 0;
}