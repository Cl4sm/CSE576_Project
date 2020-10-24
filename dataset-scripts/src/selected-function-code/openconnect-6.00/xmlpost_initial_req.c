int xmlpost_initial_req(struct openconnect_info *vpninfo, char *request_body, int req_len, int cert_fail)
{
	xmlNodePtr root, node;
	xmlDocPtr doc = xmlpost_new_query(vpninfo, "init", &root);
	char *url;
	int result;

	if (!doc)
		return -ENOMEM;

	if (vpninfo->urlpath)
		result = asprintf(&url, "https://%s/%s", vpninfo->hostname, vpninfo->urlpath);
	else
		result = asprintf(&url, "https://%s", vpninfo->hostname);

	if (result == -1)
		goto bad;
	node = xmlNewTextChild(root, NULL, XCAST("group-access"), XCAST(url));
	free(url);
	if (!node)
		goto bad;
	if (cert_fail) {
		node = xmlNewTextChild(root, NULL, XCAST("client-cert-fail"), NULL);
		if (!node)
			goto bad;
	}
	if (vpninfo->authgroup) {
		node = xmlNewTextChild(root, NULL, XCAST("group-select"), XCAST(vpninfo->authgroup));
		if (!node)
			goto bad;
	}
	return xmlpost_complete(doc, request_body, req_len);

bad:
	xmlpost_complete(doc, NULL, 0);
	return -ENOMEM;
}