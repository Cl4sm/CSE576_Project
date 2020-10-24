lookup_channel(u_char *channel, int server, int do_unlink)
{
	ChannelList	*chan, *last = (ChannelList *) 0;

	if (!channel || !*channel ||
	    (server == -1 && (server = primary_server) == -1))
		return (ChannelList *) 0;
	chan = server_list[server].chan_list;
	while (chan)
	{
		if (chan->server == server && same_channel(chan, channel))
		{
			if (do_unlink == CHAN_UNLINK)
			{
				if (last)
					last->next = chan->next;
				else
					server_list[server].chan_list = chan->next;
			}
			break;
		}
		last = chan;
		chan = chan->next;
	}
	return chan;
}
