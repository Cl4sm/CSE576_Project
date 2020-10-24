connect_to_server_direct(server_name, port, nick, server_index)
	u_char	*server_name;
	int	port;
	u_char	*nick;
	int	server_index;
{
	int	new_des;
	struct	addrinfo *r = 0, *r0 = 0;

	oper_command = 0;
	errno = 0;
#ifdef HAVE_SYS_UN_H
	if (*server_name == '/')
		new_des = connect_to_unix(port, server_name);
	else
#endif /* HAVE_SYS_UN_H */
	{
		if (server_index >= 0 && server_list[server_index].res && server_list[server_index].res0)
			new_des = connect_by_number(port, server_name, 1,
			    &server_list[server_index].res,
			    &server_list[server_index].res0);
		else
			new_des = connect_by_number(port, server_name, 1, &r, &r0);
	}
	if (new_des < 0)
	{
		char *e = NULL;
		switch (new_des)
		{
		default:
		case -2:
			e = "Unknown host";
			errno = 0;
			break;
		case -3:
			e = "socket";
			break;
		case -4:
			e = "connect";
			break;
		}
			
		say("Unable to connect to port %d of server %s: %s%s%s", port, server_name, e,
		    errno ? ": " : "", errno ? strerror(errno) : "");
		if (is_server_open(from_server))
			say("Connection to server %s resumed...", server_list[from_server].name);
		return (-1);
	}

	update_all_status();
	add_to_server_list(server_name, port, (u_char *) 0, nick, -1, get_server_version(from_server), SL_ADD_OVERWRITE);

	if (server_list[from_server].localaddr)
		new_free(&server_list[from_server].localaddr);
	server_list[from_server].localaddr = 0;

#ifdef HAVE_SYS_UN_H
	if (*server_name == '/')
	{
		server_list[from_server].localaddr = 0;
		server_list[from_server].localaddrlen = 0;
	}
	else
#endif /* HAVE_SYS_UN_H */
	{
		SOCKADDR_STORAGE *localaddr =
		    (SOCKADDR_STORAGE *) new_malloc(sizeof *localaddr);
		socklen_t address_len = sizeof *localaddr;

		if (getsockname(new_des, (struct sockaddr *) localaddr, &address_len)
		    >= 0)
		{
			server_list[from_server].localaddr = localaddr;
			server_list[from_server].localaddrlen = address_len;
		}
		else
		{
			close(new_des);
			say("Could not getsockname(): %s", strerror(errno));
			return -1;
		}
	}
	if (port)
	{
		server_list[from_server].read = new_des;
		server_list[from_server].write = new_des;
	}
	else
		server_list[from_server].read = new_des;
	if (!server_list[from_server].res0 && r && r0)
	{
		server_list[from_server].res = r;
		server_list[from_server].res0 = r0;
	}

	server_list[from_server].operator = 0;
	return (0);
}
