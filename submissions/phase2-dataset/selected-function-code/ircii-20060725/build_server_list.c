void
build_server_list(servers)
	u_char	*servers;
{
	u_char	*host,
		*rest,
		*extra,
		*mode,
		*password = (u_char *) 0,
		*port = (u_char *) 0,
		*group = (u_char *) 0,
		*nick = (u_char *) 0;
	int	port_num,
		type = -1;

	if (servers == (u_char *) 0)
		return;
	while (servers)
	{
		if ((rest = my_index(servers, '\n')) != NULL)
			*rest++ = '\0';
		while ((host = next_arg(servers, &servers)) != NULL)
		{
			parse_server_info(&host, &port, &password, &nick, &group, &extra, &type);
			if (port && *port)
			{
				port_num = my_atoi(port);
				if (!port_num)
					port_num = CHOOSE_PORT(type);
			}
			else
				port_num = CHOOSE_PORT(type);
			if (!nick)
				nick = nickname;
			add_to_server_list(host, port_num, password, nick, find_server_group(group, 1), type, 0);
			if (extra)
			{
				switch (type)
				{
				case ServerICB:
					if ((mode = my_index(extra, ':')) && mode[1])
						*mode++ = 0;
					else
						mode = NULL;
					set_server_icbgroup(from_server, extra);
					set_server_icbmode(from_server, mode);
					break;
				default:
					break;
					/* nothing yet */
				}
			}
		}
		servers = rest;
	}
}
