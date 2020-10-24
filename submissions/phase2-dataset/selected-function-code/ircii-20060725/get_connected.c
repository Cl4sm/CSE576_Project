void
get_connected(server)
	int	server;
{
	int	s,
		ret = -1;

	if (server_list)
	{
		int	already_connected = 0;

		if (server == number_of_servers)
			server = 0;
		else if (server < 0)
			server = number_of_servers - 1;
		s = server;
		if (reconnect_to_server(server, primary_server))
		{
			while (server_list[server].read == -1)
			{
				server++;
				if (server == number_of_servers)
					server = 0;
				if (server == s)
				{
					clean_whois_queue();
					say("Use /SERVER to connect to a server");
					break;
				}
				from_server = server;
				already_connected = is_server_connected(server);
				ret = reconnect_to_server(server, primary_server);
			}
			if (!ret)
				from_server = server;
			else
				from_server = -1;
		}
		if (from_server != -1) {
			int flags;

			flags = (already_connected ? 0 : WIN_TRANSFER);
			window_set_server(-1, from_server, flags);
		}
	}
	else
	{
		clean_whois_queue();
		say("Use /SERVER to connect to a server");
	}
}
