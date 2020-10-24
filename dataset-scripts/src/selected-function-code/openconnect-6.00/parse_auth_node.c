static int parse_auth_node(struct openconnect_info *vpninfo, xmlNode *xml_node,
			   struct oc_auth_form *form)
{
	int ret = 0;

	for (xml_node = xml_node->children; xml_node; xml_node = xml_node->next) {
		if (xml_node->type != XML_ELEMENT_NODE)
			continue;

		xmlnode_get_text(xml_node, "banner", &form->banner);
		xmlnode_get_text(xml_node, "message", &form->message);
		xmlnode_get_text(xml_node, "error", &form->error);

		if (xmlnode_is_named(xml_node, "form")) {

			/* defaults for new XML POST */
			form->method = strdup("POST");
			form->action = strdup("/");

			xmlnode_get_prop(xml_node, "method", &form->method);
			xmlnode_get_prop(xml_node, "action", &form->action);

			if (!form->method || !form->action ||
			    strcasecmp(form->method, "POST") || !form->action[0]) {
				vpn_progress(vpninfo, PRG_ERR,
					     _("Cannot handle form method='%s', action='%s'\n"),
					     form->method, form->action);
				ret = -EINVAL;
				goto out;
			}

			ret = parse_form(vpninfo, form, xml_node);
			if (ret < 0)
				goto out;
		} else if (!vpninfo->csd_scriptname && xmlnode_is_named(xml_node, "csd")) {
			xmlnode_get_prop(xml_node, "token", &vpninfo->csd_token);
			xmlnode_get_prop(xml_node, "ticket", &vpninfo->csd_ticket);
		}
		/* For Windows, vpninfo->csd_xmltag will be "csd" and there are *two* <csd>
		   nodes; one with token/ticket and one with the URLs. Process them both
		   the same and rely on the fact that xmlnode_get_prop() will not *clear*
		   the variable if no such property is found. */
		if (!vpninfo->csd_scriptname && xmlnode_is_named(xml_node, vpninfo->csd_xmltag)) {
			/* ignore the CSD trojan binary on mobile platforms */
			if (!vpninfo->csd_nostub)
				xmlnode_get_prop(xml_node, "stuburl", &vpninfo->csd_stuburl);
			xmlnode_get_prop(xml_node, "starturl", &vpninfo->csd_starturl);
			xmlnode_get_prop(xml_node, "waiturl", &vpninfo->csd_waiturl);
			vpninfo->csd_preurl = strdup(vpninfo->urlpath);
		}
	}

out:
	return ret;
}