remove_from_channel(u_char *channel, u_char *nick, int server)
{
	ChannelList *chan;
	NickList *tmp;

	if (channel)
	{
		if ((chan = lookup_channel(channel, server, CHAN_NOUNLINK)))
		{
			if ((tmp = (NickList *) list_lookup((List **)(void *)&(chan->nicks), nick, !USE_WILDCARDS, REMOVE_FROM_LIST)))
			{
				new_free(&(tmp->nick));
				new_free(&tmp);
			}
		}
	}
	else
	{
		for (chan = server_list[server].chan_list; chan; chan = chan->next)
		{
			if ((tmp = (NickList *) list_lookup((List **)(void *)&(chan->nicks), nick, !USE_WILDCARDS, REMOVE_FROM_LIST)))
			{
				new_free(&(tmp->nick));
				new_free(&tmp);
			}
		}
	}
}
