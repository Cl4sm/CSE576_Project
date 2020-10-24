static int xmlpost_append_form_opts(struct openconnect_info *vpninfo,
				    struct oc_auth_form *form, char *body, int bodylen)
{
	xmlNodePtr root, node;
	xmlDocPtr doc = xmlpost_new_query(vpninfo, "auth-reply", &root);
	struct oc_form_opt *opt;

	if (!doc)
		return -ENOMEM;

	if (vpninfo->opaque_srvdata) {
		node = xmlCopyNode(vpninfo->opaque_srvdata, 1);
		if (!node)
			goto bad;
		if (!xmlAddChild(root, node))
			goto bad;
	}

	node = xmlNewChild(root, NULL, XCAST("auth"), NULL);
	if (!node)
		goto bad;

	for (opt = form->opts; opt; opt = opt->next) {
		/* group_list: create a new <group-select> node under <config-auth> */
		if (!strcmp(opt->name, "group_list")) {
			if (!xmlNewTextChild(root, NULL, XCAST("group-select"), XCAST(opt->value)))
				goto bad;
			continue;
		}

		/* answer,whichpin,new_password: rename to "password" */
		if (!strcmp(opt->name, "answer") ||
		    !strcmp(opt->name, "whichpin") ||
		    !strcmp(opt->name, "new_password")) {
			if (!xmlNewTextChild(node, NULL, XCAST("password"), XCAST(opt->value)))
				goto bad;
			continue;
		}

		/* verify_pin,verify_password: ignore */
		if (!strcmp(opt->name, "verify_pin") ||
		    !strcmp(opt->name, "verify_password")) {
			continue;
		}

		/* everything else: create <foo>user_input</foo> under <auth> */
		if (!xmlNewTextChild(node, NULL, XCAST(opt->name), XCAST(opt->value)))
			goto bad;
	}

	if (vpninfo->csd_token &&
	    !xmlNewTextChild(root, NULL, XCAST("host-scan-token"), XCAST(vpninfo->csd_token)))
		goto bad;

	return xmlpost_complete(doc, body, bodylen);

bad:
	xmlpost_complete(doc, NULL, 0);
	return -ENOMEM;
}