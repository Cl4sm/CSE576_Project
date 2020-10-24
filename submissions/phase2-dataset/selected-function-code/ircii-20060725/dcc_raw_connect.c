u_char	*
dcc_raw_connect(host, iport)
	u_char	*host;
	u_int	iport;
{
	DCC_list	*Client;
	struct	addrinfo hints, *res = 0, *res0 = 0;
	struct	sockaddr_in address;
	u_char	addr[NI_MAXHOST];
	u_char	PortName[10], *RetName = (u_char *) 0;
	u_short	port = (u_short)iport;
	int	lastlog_level, err;

	lastlog_level = set_lastlog_msg_level(LOG_DCC);

	memset(&hints, 0, sizeof hints);
	hints.ai_flags = 0;
	hints.ai_protocol = 0;
	hints.ai_addrlen = 0;
	hints.ai_canonname = NULL;
	hints.ai_addr = NULL;
	hints.ai_next = NULL;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_family = AF_INET;
	err = getaddrinfo(CP(host), 0, &hints, &res0);
	if (err == 0)
	{
		for (res = res0; res; res = res->ai_next)
			if (res->ai_family == PF_INET)
			{
				bcopy(res->ai_addr, &address, sizeof address);
				break;
			}
		freeaddrinfo(res0);
	}
	if (!res || err)
	{
		say("Unknown host: %s", host);
		(void) set_lastlog_msg_level(lastlog_level);
		goto out;
	}

	snprintf(CP(PortName), sizeof PortName, "%d", port);
	Client = dcc_searchlist(host, PortName, DCC_RAW, 1, (u_char *) 0);
	if (Client->flags & DCC_ACTIVE)
	{
		say("A previous DCC RAW to %s on %s exists", host, PortName);
		(void) set_lastlog_msg_level(lastlog_level);
		return RetName;
	}
	Client->remport = port;
	err = getnameinfo((struct sockaddr *) &address, sizeof address, CP(addr), NI_MAXHOST, 0, 0, NI_NUMERICHOST);
	if (err != 0) 
	{
		my_strncpy(addr, "[unknown]", sizeof(addr) - 1);
		yell("dcc_raw_connect: getnameinfo failed: %s", gai_strerror(err));
	}
	malloc_strcpy(&Client->remname, addr);
	Client->flags = DCC_OFFER | DCC_RAW;
	if (!dcc_open(Client))
		return RetName;
	snprintf(CP(PortName), sizeof PortName, "%d", Client->read);
	malloc_strcpy(&Client->user, PortName);
	if (do_hook(DCC_RAW_LIST, "%s %s E %d", PortName, host, port))
		put_it("DCC RAW connection to %s on %s via %d established",
				host, PortName, port);
	malloc_strcpy(&RetName, PortName);
	(void) set_lastlog_msg_level(lastlog_level);
out:
	return RetName;
}
