int openconnect_set_proxy_auth(struct openconnect_info *vpninfo, char *methods)
{
	int i;
	char *p, *start = methods;

	for (i = 0; i < sizeof(auth_methods) / sizeof(auth_methods[0]); i++)
		vpninfo->auth[auth_methods[i].state_index].state = AUTH_DISABLED;

	while (methods) {
		p = strchr(methods, ',');
		if (p)
			*(p++) = 0;

		for (i = 0; i < sizeof(auth_methods) / sizeof(auth_methods[0]); i++) {
			if (!strcasecmp(methods, auth_methods[i].name) ||
			    (auth_methods[i].state_index == AUTH_TYPE_GSSAPI &&
			     !strcasecmp(methods, "gssapi"))) {
				vpninfo->auth[auth_methods[i].state_index].state = AUTH_UNSEEN;
				break;
			}
		}
		methods = p;
	}
	vpninfo->authmethods_set = 1;
	free(start);
	return 0;
}