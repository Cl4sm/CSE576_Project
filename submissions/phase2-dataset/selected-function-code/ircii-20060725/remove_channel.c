void
remove_channel(u_char *channel, int server)
{
	ChannelList *tmp;

	if (channel)
	{
		int refnum = -1;

		if ((tmp = lookup_channel(channel, server, CHAN_NOUNLINK)))
		{
			tmp = lookup_channel(channel, server, CHAN_UNLINK);
			if (tmp->window)
				refnum = tmp->window->refnum;
			free_channel(&tmp);
		}

		(void)is_current_channel(channel, server, refnum);
	}
	else
	{
		ChannelList *next;

		for (tmp = server_list[server].chan_list; tmp; tmp = next)
		{
			next = tmp->next;
			free_channel(&tmp);
		}
		server_list[server].chan_list = (ChannelList *) 0;
	}
	update_all_windows();
}
