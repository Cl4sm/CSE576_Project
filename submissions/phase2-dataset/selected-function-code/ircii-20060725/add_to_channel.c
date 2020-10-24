add_to_channel(u_char *channel, u_char *nick, int server, int oper, int voice)
{
	NickList *new;
	ChannelList *chan;
	int	ischop = oper;
	int	hasvoice = voice;

	if ((chan = lookup_channel(channel, server, CHAN_NOUNLINK)))
	{
		if (*nick == '+')
		{
			hasvoice = 1;
			nick++;
		}
		if (*nick == '@')
		{
			nick++;
			if (!my_stricmp(nick, get_server_nickname(server))
			    && !((chan->status & CHAN_NAMES)
				 && (chan->status & CHAN_MODE)))
			{
				u_char	*mode = recreate_mode(chan);

				if (*mode)
				{
					int	old_server = from_server;

					from_server = server;
					send_to_server("MODE %s %s",
					    chan->channel, mode);
					from_server = old_server;
				}
				chan->status |= CHAN_CHOP;
			}
			ischop = 1;
		}

		if ((new = (NickList *)
		     remove_from_list((List **)(void *)&(chan->nicks), nick)))
		{
			new_free(&(new->nick));
			new_free(&new);
		}
		new = (NickList *) new_malloc(sizeof(NickList));
		new->nick = (u_char *) 0;
		new->chanop = ischop;
		new->hasvoice = hasvoice;
		malloc_strcpy(&(new->nick), nick);
		add_to_list((List **)(void *)&(chan->nicks), (List *) new);
	}
	notify_mark(nick, 1, 0);
}
