int config_lookup_host(struct openconnect_info *vpninfo, const char *host)
{
	int i, ret;
	size_t size;
	char *xmlfile;
	unsigned char sha1[SHA1_SIZE];
	xmlDocPtr xml_doc;
	xmlNode *xml_node, *xml_node2;

	if (!vpninfo->xmlconfig)
		return 0;

	ret = load_xml_conf_file(vpninfo, &xmlfile, &size);
	if (ret <= 0) {
		if (ret == 0)
			fprintf(stderr, _("Treating host \"%s\" as a raw hostname\n"),
				host);
		return ret;
	}

	if (openconnect_sha1(sha1, xmlfile, size)) {
		fprintf(stderr, _("Failed to SHA1 existing file\n"));
		return -1;
	}

	for (i = 0; i < SHA1_SIZE; i++)
		snprintf(&vpninfo->xmlsha1[i*2], 3, "%02x", sha1[i]);

	vpn_progress(vpninfo, PRG_DEBUG, _("XML config file SHA1: %s\n"),
		     vpninfo->xmlsha1);

	xml_doc = xmlReadMemory(xmlfile, size, "noname.xml", NULL, 0);

	free(xmlfile);

	if (!xml_doc) {
		fprintf(stderr, _("Failed to parse XML config file %s\n"),
			vpninfo->xmlconfig);
		fprintf(stderr, _("Treating host \"%s\" as a raw hostname\n"),
			host);
		return 0;
	}
	xml_node = xmlDocGetRootElement(xml_doc);

	for (xml_node = xml_node->children; xml_node; xml_node = xml_node->next) {
		if (xml_node->type == XML_ELEMENT_NODE &&
		    !strcmp((char *)xml_node->name, "ServerList")) {

			for (xml_node = xml_node->children; xml_node && !vpninfo->hostname;
			     xml_node = xml_node->next) {

				if (xml_node->type == XML_ELEMENT_NODE &&
				    !strcmp((char *)xml_node->name, "HostEntry")) {
					int match = 0;

					for (xml_node2 = xml_node->children;
					     match >= 0 && xml_node2; xml_node2 = xml_node2->next) {

						if (xml_node2->type != XML_ELEMENT_NODE)
							continue;

						if (!match && !strcmp((char *)xml_node2->name, "HostName")) {
							char *content = (char *)xmlNodeGetContent(xml_node2);
							if (content && !strcmp(content, host))
								match = 1;
							else
								match = -1;
							free(content);
						} else if (match &&
							   !strcmp((char *)xml_node2->name, "HostAddress")) {
							char *content = (char *)xmlNodeGetContent(xml_node2);
							if (content) {
								vpninfo->hostname = content;
								printf(_("Host \"%s\" has address \"%s\"\n"),
								       host, content);
							}
						} else if (match &&
							   !strcmp((char *)xml_node2->name, "UserGroup")) {
							char *content = (char *)xmlNodeGetContent(xml_node2);
							if (content) {
								free(vpninfo->urlpath);
								vpninfo->urlpath = content;
								printf(_("Host \"%s\" has UserGroup \"%s\"\n"),
								       host, content);
							}
						}
					}
				}

			}
			break;
		}
	}
	xmlFreeDoc(xml_doc);

	if (!vpninfo->hostname) {
		fprintf(stderr, _("Host \"%s\" not listed in config; treating as raw hostname\n"),
			host);
	}

	return 0;
}