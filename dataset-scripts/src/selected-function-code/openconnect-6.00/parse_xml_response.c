int parse_xml_response(struct openconnect_info *vpninfo, char *response, struct oc_auth_form **formp, int *cert_rq)
{
	struct oc_auth_form *form;
	xmlDocPtr xml_doc;
	xmlNode *xml_node;
	int ret;

	if (*formp) {
		free_auth_form(*formp);
		*formp = NULL;
	}
	if (cert_rq)
		*cert_rq = 0;

	if (!response) {
		vpn_progress(vpninfo, PRG_DEBUG,
			     _("Empty response from server\n"));
		return -EINVAL;
	}

	form = calloc(1, sizeof(*form));
	if (!form)
		return -ENOMEM;
	xml_doc = xmlReadMemory(response, strlen(response), "noname.xml", NULL,
				XML_PARSE_NOERROR|XML_PARSE_RECOVER);
	if (!xml_doc) {
		vpn_progress(vpninfo, PRG_ERR,
			     _("Failed to parse server response\n"));
		vpn_progress(vpninfo, PRG_DEBUG,
			     _("Response was:%s\n"), response);
		free(form);
		return -EINVAL;
	}

	xml_node = xmlDocGetRootElement(xml_doc);
	while (xml_node) {
		ret = 0;

		if (xml_node->type != XML_ELEMENT_NODE) {
			xml_node = xml_node->next;
			continue;
		}
		if (xmlnode_is_named(xml_node, "config-auth")) {
			/* if we do have a config-auth node, it is the root element */
			xml_node = xml_node->children;
			continue;
		} else if (xmlnode_is_named(xml_node, "client-cert-request")) {
			if (cert_rq)
				*cert_rq = 1;
			else {
				vpn_progress(vpninfo, PRG_ERR,
					     _("Received <client-cert-request> when not expected.\n"));
				ret = -EINVAL;
			}
		} else if (xmlnode_is_named(xml_node, "auth")) {
			xmlnode_get_prop(xml_node, "id", &form->auth_id);
			ret = parse_auth_node(vpninfo, xml_node, form);
		} else if (xmlnode_is_named(xml_node, "opaque")) {
			if (vpninfo->opaque_srvdata)
				xmlFreeNode(vpninfo->opaque_srvdata);
			vpninfo->opaque_srvdata = xmlCopyNode(xml_node, 1);
			if (!vpninfo->opaque_srvdata)
				ret = -ENOMEM;
		} else if (xmlnode_is_named(xml_node, "host-scan")) {
			ret = parse_host_scan_node(vpninfo, xml_node);
		} else if (xmlnode_is_named(xml_node, "config")) {
			parse_config_node(vpninfo, xml_node);
		} else {
			xmlnode_get_text(xml_node, "session-token", &vpninfo->cookie);
			xmlnode_get_text(xml_node, "error", &form->error);
		}

		if (ret)
			goto out;
		xml_node = xml_node->next;
	}

	if (!form->auth_id && (!cert_rq || !*cert_rq)) {
		vpn_progress(vpninfo, PRG_ERR,
			     _("XML response has no \"auth\" node\n"));
		ret = -EINVAL;
		goto out;
	}

	*formp = form;
	xmlFreeDoc(xml_doc);
	return 0;

 out:
	xmlFreeDoc(xml_doc);
	free_auth_form(form);
	return ret;
}