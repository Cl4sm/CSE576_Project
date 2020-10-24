int
connect_to_server(server_name, port, nick, c_server)
	u_char	*server_name;
	int	port;
	u_char	*nick;
	int	c_server;
{
	int	server_index;
	SOCKADDR_STORAGE	sa;
	socklen_t salen = sizeof sa;
	int rv;

	save_message_from();
	message_from((u_char *) 0, LOG_CURRENT);
	server_index = find_in_server_list(server_name, port, nick);
	if (server_index < 0)
	{
		yell("connect_to_server: server_index returned -1 from find_in_server_list()");
		yell("aborting!");
		abort();
	}
	server_list[server_index].attempting_to_connect = 1;
	/*
	 * check if the server doesn't exist, or that we're not already
	 * connected to it.
	 */
	if (!is_server_connected(server_index))
	{
		if (is_server_open(server_index))
			close_server(server_index, empty_string);
		if (port == -1)
			port = server_list[server_index].port;
		if (port == -1)
			port = CHOOSE_PORT(server_list[server_index].version);
		say("Connecting to port %d of server %s", port, server_name);
		
		if (!qflag)
			load_ircquick();
		
		if (using_server_process)
			rv = connect_to_server_process(server_name, port, nick, server_index);
		else
			rv = connect_to_server_direct(server_name, port, nick, server_index);
		if (rv)
		{
			server_list[server_index].attempting_to_connect = 0;
			restore_message_from();
			return -1;
		}
		if ((c_server != -1) && (c_server != from_server))
		{
#ifdef NON_BLOCKING_CONNECTS
#if defined(GKM)
			say("--- server %s will be closed when we connect", server_list[c_server].name);
			if (server_list[c_server].flags & CLOSE_PENDING)
				say("--- why are we flagging this for closing a second time?");
#endif /* GKM */
			server_list[from_server].close_serv = c_server;
			server_list[c_server].flags |= CLOSE_PENDING;
			server_list[c_server].connected = 0;
#else
			close_server(c_server, empty_string);
#endif /* NON_BLOCKING_CONNECTS */
		}
		else
		{
			server_list[from_server].close_serv = -1;
		}
		if (server_list[from_server].nickname == (u_char *) 0)
			malloc_strcpy(&server_list[from_server].nickname, nickname);
		server_list[from_server].flags &= ~LOGGED_IN;
		/*
		 * this used to be an ifndef NON_BLOCKING_CONNECTS .. we want to do this
		 * whenever the connection is valid, it's possible for a connect to be
		 * "immediate".
		 */
		if (is_server_open(from_server) &&
		    (using_server_process ||
		    getpeername(server_list[from_server].read, (struct sockaddr *) &sa, &salen) != -1))
			login_to_server(from_server);
	}
	else
	{
		if (port == -1)
		{
			if (server_index != -1)
				port = server_list[server_index].port;
			else
				port = CHOOSE_PORT(get_server_version(server_index));
		}
		say("Connected to port %d of server %s", port, server_name);
		from_server = server_index;
		if ((c_server != -1) && (c_server != from_server))
			close_server(c_server, empty_string);
	}
	update_all_status();
	restore_message_from();
	return 0;
}
