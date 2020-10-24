static xmlDocPtr xmlpost_new_query(struct openconnect_info *vpninfo, const char *type,
				   xmlNodePtr *rootp)
{
	xmlDocPtr doc;
	xmlNodePtr root, node;

	doc = xmlNewDoc(XCAST("1.0"));
	if (!doc)
		return NULL;

	*rootp = root = xmlNewNode(NULL, XCAST("config-auth"));
	if (!root)
		goto bad;
	if (!xmlNewProp(root, XCAST("client"), XCAST("vpn")))
		goto bad;
	if (!xmlNewProp(root, XCAST("type"), XCAST(type)))
		goto bad;
	xmlDocSetRootElement(doc, root);

	node = xmlNewTextChild(root, NULL, XCAST("version"), XCAST(openconnect_version_str));
	if (!node)
		goto bad;
	if (!xmlNewProp(node, XCAST("who"), XCAST("vpn")))
		goto bad;

	node = xmlNewTextChild(root, NULL, XCAST("device-id"), XCAST(vpninfo->platname));
	if (!node)
		goto bad;
	if (vpninfo->mobile_platform_version) {
		if (!xmlNewProp(node, XCAST("platform-version"), XCAST(vpninfo->mobile_platform_version)) ||
		    !xmlNewProp(node, XCAST("device-type"), XCAST(vpninfo->mobile_device_type)) ||
		    !xmlNewProp(node, XCAST("unique-id"), XCAST(vpninfo->mobile_device_uniqueid)))
			goto bad;
	}

	return doc;

bad:
	xmlFreeDoc(doc);
	return NULL;
}