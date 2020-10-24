void
servercmd(command, args, subargs)
	u_char	*command,
		*args,
		*subargs;
{
	u_char	*server,
		*port,
		*extra,
		*newmode,
		*password = (u_char *) 0,
		*nick = (u_char *) 0,
		*group = (u_char *) 0;
	int	port_num,
		i,
		new_server_flags,
		type = -1;

	if ((server = next_arg(args, &args)) != NULL)
	{
		while (*server == '-')
		{
			size_t	len;

			/*
			 * old usage of `/server -' handled here.
			 */
			if (*++server == '\0')
			{
				get_connected(primary_server - 1);
				return;
			}
			upper(server);
			len = my_strlen(server);
			/*
			 * just don't return if you want to perform some action in one of
			 * the flag handling sections.
			 */
			if (!my_strncmp(server, "ICB", len))
				type = ServerICB;
			else if (!my_strncmp(server, "IRC", len))
				type = DEFAULT_SERVER_VERSION;
			else if (!my_strncmp(server, "DELETE", len))
			{
				if ((server = next_arg(args, &args)) != NULL)
				{
					if ((i = parse_server_index(server)) == -1)
					{
						if (-1 == (i = find_in_server_list(server, 0, 0)))
						{
							say("No such server in list");
							return;
						}
					}
					if (server_list[i].connected)
					{
						say("Can not delete server that is already open");
						return;
					}
					remove_from_server_list(i);
					return;
				}
				say("Need server number for -DELETE");
				return;
			}
			else if (!my_strncmp(server, "GROUP", len))
			{
				if ((group = next_arg(args, &args)) == NULL)
				{
					say("SERVER -GROUP needs <group> and <server>");
					return;
				}
			}
			else
			{
				say("SERVER: %s is an unknown flag", server);
				return;
			}
			if ((server = next_arg(args, &args)) == NULL)
			{
				say("SERVER: need a server name");
				return;
			}
		}

		if (my_index(server, ':') != NULL)
		{
			parse_server_info(&server, &port, &password, &nick,
			    group ? 0 : &group, &extra, &type);
			if (!my_strlen(server))
			{
				say("Server name required");
				return;
			}
			if (port && *port) {
				port_num = my_atoi(port);
				if (!port_num)
					port_num = CHOOSE_PORT(type);
			} else
				port_num = CHOOSE_PORT(type);
		}
		else
		{
			if ((port = next_arg(args, &args)) != NULL)
			{
				port_num = my_atoi(port);
				if (!port_num)
					port_num = CHOOSE_PORT(type);
				if ((password = next_arg(args, &args)) != NULL)
					nick = next_arg(args, &args);
			}
			else
				port_num = CHOOSE_PORT(type);

			extra = (u_char *) 0;
		}

		add_to_server_list(server, port_num, password, nick, -1, type, 0);

		if (group && *group)
			server_list[from_server].server_group = find_server_group(group, 1);

		if (extra && type == ServerICB)
		{
			if ((newmode = my_index(extra, ':')))
			{
				*newmode++ = 0;
				malloc_strcpy(&(server_list[from_server].icbmode), newmode);
			}
			malloc_strcpy(&(server_list[from_server].group), extra);
		}

		if (*server == '+' || *server == '=' || *server == '~')
		{
			if (group)
				add_server_to_server_group(curr_scr_win->server,
				    group);

			if (*(server+1))
			{
				u_char	servinfo[INPUT_BUFFER_SIZE+1];

				if (*server == '+')
					server++;
				/* Reconstitute whole server info so
				  window_get_connected can parse it -Sol */
				snprintf(CP(servinfo), sizeof servinfo, "%s:%d:%s:%s",
					server, port_num,
					password ? password : empty_string,
					nick ? nick : empty_string);
				window_get_connected(curr_scr_win, servinfo, -1,
				    (u_char *) 0, group, type);
			}
			else
				get_connected(primary_server + 1);
			return;
		}
		/*
		 * work in progress.. window->prev_server needs to be set for
		 * all windows that used to be associated with a server as it
		 * switches [successfully] to a new server.
		 * this'll be fun since that can happen in server.c and
		 * window.c and non-blocking-connects will throw yet another
		 * wrench into things since we only want it to happen on
		 * a successful connect. - gkm
		 */
		else if (*server == '.')
		{
			if (*(++server))
			{
				say("syntax error - nothing may be specified after the '.'");
				return;
			}
			if (current_screen && curr_scr_win &&
			    curr_scr_win->prev_server != -1)
			{
				if (group)
					add_server_to_server_group(curr_scr_win->prev_server, group);

				window_restore_server(curr_scr_win->prev_server);
				window_get_connected(curr_scr_win, NULL,
				    curr_scr_win->server, (u_char *) 0, group, type);
			}
			else
				say("No server previously in use in this window");
			return;
		}
		if ((i = parse_server_index(server)) != -1)
		{
			server = server_list[i].name;
			if (server_list[i].port != -1)
				port_num = server_list[i].port;
			if (server_list[i].nickname && !nick)
				nick = server_list[i].nickname;
		}
		else
			i = find_in_server_list(server, port_num, nick);

		if (group)
			add_server_to_server_group(i, group);

		if (is_server_connected(i))
		{
			/*
			 * We reset the log level only if the "new" server
			 * already has windows associated with it : here it's
			 * equivalent to its already being connected. -Sol
			 */
			new_server_flags = 0;
		}
		else
			new_server_flags = WIN_TRANSFER;
		if (connect_to_server(server, port_num, nick, primary_server) != -1)
		{
			if (primary_server > -1 && from_server != primary_server &&
			    !server_list[from_server].away &&
			    server_list[primary_server].away)
				malloc_strcpy(&server_list[from_server].away,
				    server_list[primary_server].away);
			window_set_server(-1, from_server, new_server_flags);
		}
	}
	else
		display_server_list();
}
