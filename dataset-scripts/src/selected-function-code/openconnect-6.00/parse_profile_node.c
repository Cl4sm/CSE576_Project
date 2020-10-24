static void parse_profile_node(struct openconnect_info *vpninfo, xmlNode *xml_node)
{
	/* ignore this whole section if we already have a URL */
	if (vpninfo->profile_url && vpninfo->profile_sha1)
		return;

	/* Find <vpn rev="1.0"> child... */
	xml_node = xml_node->children;
	while (1) {
		if (!xml_node)
			return;

		if (xml_node->type == XML_ELEMENT_NODE &&
		    xmlnode_is_named(xml_node, "vpn") &&
		    !xmlnode_match_prop(xml_node, "rev", "1.0"))
			break;

		xml_node = xml_node->next;
	}

	/* Find <file type="profile" service-type="user"> */
	xml_node = xml_node->children;
	while (1) {
		if (!xml_node)
			return;

		if (xml_node->type == XML_ELEMENT_NODE &&
		    xmlnode_is_named(xml_node, "file") &&
		    !xmlnode_match_prop(xml_node, "type", "profile") &&
		    !xmlnode_match_prop(xml_node, "service-type", "user"))
			break;

		xml_node = xml_node->next;
	}

	for (xml_node = xml_node->children; xml_node; xml_node = xml_node->next) {
		if (xml_node->type != XML_ELEMENT_NODE)
			continue;

		xmlnode_get_text(xml_node, "uri", &vpninfo->profile_url);
		/* FIXME: Check for <hash type="sha1"> */
		xmlnode_get_text(xml_node, "hash", &vpninfo->profile_sha1);
	}
}