list_channels(void)
{
	ChannelList *tmp;
	int	server,
		no = 1;
	int	first;

	if (connected_to_server < 1)
	{
		say("You are not connected to a server, use /SERVER to connect.");
		return;
	}
	if (get_channel_by_refnum(0))
		say("Current channel %s", get_channel_by_refnum(0));
	else
		say("No current channel for this window");
	first = 1;
	for (tmp = server_list[get_window_server(0)].chan_list; tmp; tmp = tmp->next)
	{
		if (tmp->connected != CHAN_JOINED)
			continue;
		if (first)
			say("You are on the following channels:");
		show_channel(tmp);
		first = 0;
		no = 0;
	}

	if (connected_to_server > 1)
	{
		for (server = 0; server < number_of_servers; server++)
		{
			if (server == get_window_server(0))
				continue;
			first = 1;
			for (tmp = server_list[server].chan_list; tmp; tmp = tmp->next)
			{
				if (tmp->connected != CHAN_JOINED)
					continue;
				if (first)
					say("Other servers:");
				show_channel(tmp);
				first = 0;
				no = 0;
			}
		}
	}
	if (no)
		say("You are not on any channels");
}
