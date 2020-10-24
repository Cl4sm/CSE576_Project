connect_by_number(service, host)
	char	*service;
	char	*host;
{
	int	s = -1, err;
	char	buf[256];
	struct	addrinfo hints, *res = 0, *res0 = 0;

	if (host == (char *) 0)
	{
		gethostname(buf, sizeof(buf));
		host = buf;
	}
	memset(&hints, 0, sizeof hints);
	hints.ai_flags = 0;
	hints.ai_protocol = 0;
	hints.ai_addrlen = 0;
	hints.ai_canonname = NULL;
	hints.ai_addr = NULL;
	hints.ai_next = NULL;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_family = AF_UNSPEC;
	err = getaddrinfo(host, service, &hints, &res0);
	if (err != 0) 
		return -2;
	for (res = res0; res; res = res->ai_next)
	{
		err = 0;
		if ((s = socket(res->ai_family, res->ai_socktype,
		    res->ai_protocol)) < 0)
			continue;
		set_socket_options(s);
		err = connect(s, res->ai_addr, res->ai_addrlen);
		if (err == 0)
			break;
		close(s);
	}
	freeaddrinfo(res0);
	if (err)
		return -4;
	return (s);
}
