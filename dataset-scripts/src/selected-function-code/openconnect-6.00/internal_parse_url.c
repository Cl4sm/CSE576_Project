int internal_parse_url(char *url, char **res_proto, char **res_host,
		       int *res_port, char **res_path, int default_port)
{
	char *proto = url;
	char *host, *path, *port_str;
	int port;

	host = strstr(url, "://");
	if (host) {
		*host = 0;
		host += 3;

		if (!strcasecmp(proto, "https"))
			port = 443;
		else if (!strcasecmp(proto, "http"))
			port = 80;
		else if (!strcasecmp(proto, "socks") ||
			 !strcasecmp(proto, "socks4") ||
			 !strcasecmp(proto, "socks5"))
			port = 1080;
		else
			return -EPROTONOSUPPORT;
	} else {
		if (default_port) {
			proto = NULL;
			port = default_port;
			host = url;
		} else
			return -EINVAL;
	}

	path = strchr(host, '/');
	if (path)
		*(path++) = 0;

	port_str = strrchr(host, ':');
	if (port_str) {
		char *end;
		int new_port = strtol(port_str + 1, &end, 10);

		if (!*end) {
			*port_str = 0;
			port = new_port;
		}
	}

	if (res_proto)
		*res_proto = proto ? strdup(proto) : NULL;
	if (res_host)
		*res_host = strdup(host);
	if (res_port)
		*res_port = port;
	if (res_path)
		*res_path = (path && *path) ? strdup(path) : NULL;

	/* Undo the damage we did to the original string */
	if (port_str)
		*(port_str) = ':';
	if (path)
		*(path - 1) = '/';
	if (proto)
		*(host - 3) = ':';
	return 0;
}