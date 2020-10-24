static int parse_form(struct openconnect_info *vpninfo, struct oc_auth_form *form,
		      xmlNode *xml_node)
{
	char *input_type, *input_name, *input_label;

	for (xml_node = xml_node->children; xml_node; xml_node = xml_node->next) {
		struct oc_form_opt *opt, **p;

		if (xml_node->type != XML_ELEMENT_NODE)
			continue;

		if (!strcmp((char *)xml_node->name, "select")) {
			if (parse_auth_choice(vpninfo, form, xml_node))
				return -EINVAL;
			continue;
		}
		if (strcmp((char *)xml_node->name, "input")) {
			vpn_progress(vpninfo, PRG_DEBUG,
				     _("name %s not input\n"), xml_node->name);
			continue;
		}

		input_type = (char *)xmlGetProp(xml_node, (unsigned char *)"type");
		if (!input_type) {
			vpn_progress(vpninfo, PRG_INFO,
				     _("No input type in form\n"));
			continue;
		}

		if (!strcmp(input_type, "submit") || !strcmp(input_type, "reset")) {
			free(input_type);
			continue;
		}

		input_name = (char *)xmlGetProp(xml_node, (unsigned char *)"name");
		if (!input_name) {
			vpn_progress(vpninfo, PRG_INFO,
				     _("No input name in form\n"));
			free(input_type);
			continue;
		}
		input_label = (char *)xmlGetProp(xml_node, (unsigned char *)"label");

		opt = calloc(1, sizeof(*opt));
		if (!opt) {
			free(input_type);
			free(input_name);
			free(input_label);
			return -ENOMEM;
		}

		opt->name = input_name;
		opt->label = input_label;
		opt->flags = prop_equals(xml_node, "second-auth", "1") ? OC_FORM_OPT_SECOND_AUTH : 0;

		if (!strcmp(input_type, "hidden")) {
			opt->type = OC_FORM_OPT_HIDDEN;
			opt->value = (char *)xmlGetProp(xml_node, (unsigned char *)"value");
		} else if (!strcmp(input_type, "text")) {
			opt->type = OC_FORM_OPT_TEXT;
		} else if (!strcmp(input_type, "password")) {
			if (vpninfo->token_mode != OC_TOKEN_MODE_NONE &&
			    (can_gen_tokencode(vpninfo, form, opt) == 0)) {
				opt->type = OC_FORM_OPT_TOKEN;
			} else {
				opt->type = OC_FORM_OPT_PASSWORD;
			}
		} else {
			vpn_progress(vpninfo, PRG_INFO,
				     _("Unknown input type %s in form\n"),
				     input_type);
			free(input_type);
			free(input_name);
			free(input_label);
			free(opt);
			continue;
		}

		free(input_type);

		p = &form->opts;
		while (*p)
			p = &(*p)->next;

		*p = opt;
	}

	return 0;
}