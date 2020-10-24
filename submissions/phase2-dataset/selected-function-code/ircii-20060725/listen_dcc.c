int
listen_dcc(src_host)
	u_char	*src_host;
{
	SOCKADDR_STORAGE *ss;
	struct addrinfo hints, *res, *res0;
	int err, s;

	if (!get_int_var(BIND_LOCAL_DCCHOST_VAR))
		src_host = (u_char *) 0;
	ss = get_server_localaddr(from_server);
	memset(&hints, 0, sizeof hints);
	hints.ai_flags = AI_PASSIVE;
	hints.ai_protocol = 0;
	hints.ai_addrlen = 0;
	hints.ai_canonname = NULL;
	hints.ai_addr = NULL;
	hints.ai_next = NULL;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_family = AF_UNSPEC;
	errno = 0;
	err = getaddrinfo(CP(src_host), CP(zero), &hints, &res0);
	if (err != 0) 
	{
		errno = err;
		return -2;
	}
	for (res = res0; res; res = res->ai_next) {
		if (ss && SS_FAMILY(ss) != res->ai_family)
			continue;

		if ((s = socket(res->ai_family, res->ai_socktype,
		    res->ai_protocol)) < 0)
			continue;
		set_socket_options(s);
		if (res->ai_family == AF_INET)
			((struct sockaddr_in *)res->ai_addr)->sin_port = htons(get_int_var(DCCPORT_VAR));
#ifdef INET6
		else if (res->ai_family == AF_INET6)
			((struct sockaddr_in6 *)res->ai_addr)->sin6_port = htons(get_int_var(DCCPORT_VAR));
#endif
		if (bind(s, res->ai_addr, res->ai_addrlen) == 0 &&
		    listen(s, 1) == 0)
		{
			freeaddrinfo(res0);
			return s;
		}
		close(s);
	}
	
	freeaddrinfo(res0);
	return -1;
}
