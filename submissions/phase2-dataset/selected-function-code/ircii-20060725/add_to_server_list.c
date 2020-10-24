void
add_to_server_list(server, port, password, nick, group, type, flags)
	u_char	*server;
	int	port;
	u_char	*password;
	u_char	*nick;
	int	group;
	int	type;
	int	flags;
{
	int	i;

	if (port == -1)
		port = CHOOSE_PORT(type);
	if ((from_server = find_in_server_list(server, port, nick)) == -1)
	{
		from_server = number_of_servers++;
		if (server_list)
			server_list = (Server *) new_realloc(UP(server_list), number_of_servers * sizeof(Server));
		else
			server_list = (Server *) new_malloc(number_of_servers * sizeof(Server));
		server_list[from_server].name = (u_char *) 0;
		server_list[from_server].itsname = (u_char *) 0;
		server_list[from_server].password = (u_char *) 0;
		server_list[from_server].away = (u_char *) 0;
		server_list[from_server].version_string = (u_char *) 0;
		server_list[from_server].operator = 0;
		server_list[from_server].read = -1;
		server_list[from_server].write = -1;
		server_list[from_server].pid = -1;
		server_list[from_server].whois = 0;
		server_list[from_server].flags = SERVER_2_6_2;
		server_list[from_server].nickname = (u_char *) 0;
		server_list[from_server].connected = 0;
		server_list[from_server].eof = 0;
		server_list[from_server].motd = 1;
		server_list[from_server].group = (u_char *) 0;
		server_list[from_server].icbmode = (u_char *) 0;
		server_list[from_server].chan_list = (ChannelList *) 0;
		malloc_strcpy(&(server_list[from_server].name), server);
		if (password && *password)
			malloc_strcpy(&server_list[from_server].password, password);
		if (nick && *nick)
			malloc_strcpy(&server_list[from_server].nickname, nick);
		server_list[from_server].port = port;
		server_list[from_server].WQ_head = (WhoisQueue *) 0;
		server_list[from_server].WQ_tail = (WhoisQueue *) 0;
		server_list[from_server].whois_stuff.nick = (u_char *) 0;
		server_list[from_server].whois_stuff.user = (u_char *) 0;
		server_list[from_server].whois_stuff.host = (u_char *) 0;
		server_list[from_server].whois_stuff.channel = (u_char *) 0;
		server_list[from_server].whois_stuff.channels = (u_char *) 0;
		server_list[from_server].whois_stuff.name = (u_char *) 0;
		server_list[from_server].whois_stuff.server = (u_char *) 0;
		server_list[from_server].whois_stuff.server_stuff = (u_char *) 0;
		server_list[from_server].whois_stuff.away = (u_char *) 0;
		server_list[from_server].whois_stuff.oper = 0;
		server_list[from_server].whois_stuff.chop = 0;
		server_list[from_server].whois_stuff.not_on = 0;
		server_list[from_server].buffer = (u_char *) 0;
		server_list[from_server].close_serv = -1;
		server_list[from_server].localaddr = 0;
		server_list[from_server].localaddrlen = 0;
		switch (type)
		{
		case ServerICB:
			server_list[from_server].parse_server = icb_parse_server;
			break;
		case -1:
			/* default */
			if (client_default_icb)
			{
				type = ServerICB;
				server_list[from_server].parse_server = icb_parse_server;
				break;
			}
			type = DEFAULT_SERVER_VERSION;
			/* FALLTHROUGH */
		default:
			server_list[from_server].parse_server = irc2_parse_server;
		}
		server_list[from_server].version = type;
		server_list[from_server].ctcp_last_reply_time = 0;
		server_list[from_server].ctcp_flood_time = 0;
		server_list[from_server].ctcp_backlog_size = get_int_var(CTCP_REPLY_BACKLOG_SECONDS_VAR);
		server_list[from_server].ctcp_send_size =
			(int *)new_malloc(server_list[from_server].ctcp_backlog_size*sizeof(int));

		for(i = 0; i<server_list[from_server].ctcp_backlog_size; i++)
			server_list[from_server].ctcp_send_size[i] = 0;
		if (group == -1)
			server_list[from_server].server_group = 0;
		else
			server_list[from_server].server_group = group;
		server_list[from_server].res = 0;
		server_list[from_server].res0 = 0;
	}
	else
	{
		if (flags & SL_ADD_OVERWRITE)
		{
			server_list[from_server].port = port;
			if (password)
			{
				if (*password)
					malloc_strcpy(&(server_list[from_server].password), password);
				else
					new_free(&(server_list[from_server].password));
			}
			if (nick && *nick)
				malloc_strcpy(&(server_list[from_server].nickname), nick);
			if (group != -1)
				server_list[from_server].server_group = group;
		}
		if (server_list[from_server].res0)
		{
			freeaddrinfo(server_list[from_server].res0);
			server_list[from_server].res0 = 0;
		}
		server_list[from_server].res = 0;
		if ((int) my_strlen(server) > (int) my_strlen(server_list[from_server].name))
			malloc_strcpy(&(server_list[from_server].name), server);
	}
}
