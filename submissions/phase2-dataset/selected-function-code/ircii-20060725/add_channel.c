add_channel(u_char *channel, u_char *key, int server, int connected,
	    ChannelList *copy)
{
	ChannelList *new;
	int	do_add = 0;

	/*
	 * avoid adding channel "0"
	 */
	if (channel[0] == '0' && channel[1] == '\0')
		return;

	if ((new = lookup_channel(channel, server, CHAN_NOUNLINK)) == (ChannelList *) 0)
	{
		new = (ChannelList *) new_malloc(sizeof(ChannelList));
		new->channel = (u_char *) 0;
		new->status = 0;
		new->key = 0;
		if (key)
			malloc_strcpy(&new->key, key);
		new->nicks = (NickList *) 0;
		new->s_mode = empty_string;
		malloc_strcpy(&new->channel, channel);
		new->mode = 0;
		new->limit = 0;
		if ((new->window = is_bound(channel, server)) == (Window *) 0)
			new->window = curr_scr_win;
		do_add = 1;
		add_to_list((List **)(void *)&server_list[server].chan_list, (List *) new);
	}
	else
	{
		if (new->connected != CHAN_LIMBO && new->connected != CHAN_JOINING)
			yell("--- add_channel: add_channel found channel not CHAN_LIMBO/JOINING: %s", new->channel);
	}
	if (do_add || (connected == CHAN_JOINED))
	{
		new->server = server;
		clear_channel(new);
	}
	if (copy)
	{
		new->mode = copy->mode;
		new->limit = copy->limit;
		new->window = copy->window;
		malloc_strcpy(&new->key, copy->key);
	}
	new->connected = connected;
	if ((connected == CHAN_JOINED) && !is_current_channel(channel, server, 0))
	{
		int	flag = 1;
		Window	*tmp, *expected,
			*possible = (Window *) 0;

		expected = new->window;

		while ((tmp = traverse_all_windows(&flag)))
		{
			if (tmp->server == server)
			{
				if (tmp == expected)
				{	
					set_channel_by_refnum(tmp->refnum, channel);
					new->window = tmp;
					update_all_status();
					return;
				}
				else if (!possible)
					possible = tmp;
			}
		}
		if (possible)
		{
			set_channel_by_refnum(possible->refnum, channel);
			new->window = possible;
			update_all_status();
			return;
		}
		set_channel_by_refnum(0, channel);
		new->window = curr_scr_win;
	}
	update_all_windows();
}
