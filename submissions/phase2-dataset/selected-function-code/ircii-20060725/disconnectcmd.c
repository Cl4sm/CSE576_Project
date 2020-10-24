void
disconnectcmd(command, args, subargs)
	u_char	*command,
		*args,
		*subargs;
{
	u_char	*server;
	u_char	*message;
	int	i;
	int	old_serv;

	if ((server = next_arg(args, &args)) != NULL && server[0] != '*' && server[1] != '\0')
	{
		i = parse_server_index(server);
		if (-1 == i)
		{
			say("No such server!");
			return;
		}
	}
	else
		i = get_window_server(0);
	/*
	 * XXX - this is a major kludge.  i should never equal -1 at
	 * this point.  we only do this because something has gotten
	 * *really* confused at this point.  .mrg.
	 */
	if (i == -1)
	{
		for (i = 0; i < number_of_servers; i++)
		{
			server_list[i].eof = -1;
			server_list[i].connected = 0;
			new_close(server_list[i].read);
			new_close(server_list[i].write);
		}
		goto done;
	}
	if (!args || !*args)
		message = UP("Disconnecting");
	else
		message = args;
	if (-1 == server_list[i].write)
	{
		say("That server isn't connected!");
		return;
	}
	server = server_list[i].itsname ? server_list[i].itsname :
		server_list[i].name ? server_list[i].name : (u_char *) "unknown?";
	say("Disconnecting from server %s", server);
	old_serv = server_list[i].close_serv;
	close_server(i, message);
	server_list[i].eof = 1;
	if (old_serv != -1 && old_serv != i)
	{
		Window *tmp;
		Win_Trav stuff;

		say("Connection to server %s resumed...", server_list[old_serv].name);
		server_list[i].close_serv = -1;
		server_list[old_serv].flags &= ~(CLOSE_PENDING|CLEAR_PENDING);
		server_list[old_serv].flags |= LOGGED_IN;
		server_list[old_serv].connected = 1;
		stuff.flag = 1;
		while ((tmp = window_traverse(&stuff)))
			if (tmp->server == i)
			{
				window_set_server(tmp->refnum, old_serv, WIN_ALL);
				break;
			}
	}
done:
	clean_whois_queue();
	window_check_servers();
	if (!connected_to_server)
		say("You are not connected to a server. Use /SERVER to connect.");
}
