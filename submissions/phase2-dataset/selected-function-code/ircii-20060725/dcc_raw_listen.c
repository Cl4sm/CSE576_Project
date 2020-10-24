u_char	*
dcc_raw_listen(iport)
	u_int	iport;
{
	DCC_list	*Client;
	u_char	PortName[10];
	struct sockaddr_in locaddr;	/* XXX DCC IPv6: this one doesn't matter; for now only support DCC RAW for ipv4 */
	u_char	*RetName = NULL;
	socklen_t	size;
	int	lastlog_level;
	u_short	port = (u_short) iport;

	lastlog_level = set_lastlog_msg_level(LOG_DCC);
	if (port && port < 1025)
	{
		say("Cannot bind to a privileged port");
		(void) set_lastlog_msg_level(lastlog_level);
		return NULL;
	}
	snprintf(CP(PortName), sizeof PortName, "%d", port);
	Client = dcc_searchlist(UP("raw_listen"), PortName, DCC_RAW_LISTEN, 1, (u_char *) 0);
	if (Client->flags & DCC_ACTIVE)
	{
		say("A previous DCC RAW_LISTEN on %s exists", PortName);
		(void) set_lastlog_msg_level(lastlog_level);
		return RetName;
	}
	if (0 > (Client->read = socket(AF_INET, SOCK_STREAM, 0)))
	{
		dcc_erase(Client);
		say("socket() failed: %s", strerror(errno));
		(void) set_lastlog_msg_level(lastlog_level);
		return RetName;
	}
	set_socket_options(Client->read);
	bzero((char *) &locaddr, sizeof(locaddr));
	locaddr.sin_family = AF_INET;
	locaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	locaddr.sin_port = htons(port);
	if (bind(Client->read, (struct sockaddr *) &locaddr, sizeof(locaddr))
				== -1)
	{
		dcc_erase(Client);
		say("Could not bind port: %s", strerror(errno));
		(void) set_lastlog_msg_level(lastlog_level);
		return RetName;
	}
	listen(Client->read, 4);
	size = sizeof(locaddr);
	Client->starttime = time((time_t *) 0);
	getsockname(Client->read, (struct sockaddr *) &locaddr, &size);
	Client->write = ntohs(locaddr.sin_port);
	Client->flags |= DCC_ACTIVE;
	snprintf(CP(PortName), sizeof PortName, "%d", Client->write);
	malloc_strcpy(&Client->user, PortName);
	malloc_strcpy(&RetName, PortName);
	(void) set_lastlog_msg_level(lastlog_level);
	return RetName;
}
