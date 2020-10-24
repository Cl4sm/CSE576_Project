show_channel(ChannelList *chan)
{
	NickList *tmp;
	int	buffer_len,
		len;
	u_char	*nicks = (u_char *) 0;
	u_char	*s;
	u_char	buffer[BIG_BUFFER_SIZE];

	s = recreate_mode(chan);
	*buffer = (u_char) 0;
	buffer_len = 0;
	for (tmp = chan->nicks; tmp; tmp = tmp->next)
	{
		len = my_strlen(tmp->nick);
		if (buffer_len + len >= (sizeof(buffer) / 2))
		{
			malloc_strcpy(&nicks, buffer);
			say("\t%s +%s (%s): %s", chan->channel, s, get_server_name(chan->server), nicks);
			*buffer = (u_char) 0;
			buffer_len = 0;
		}
		my_strmcat(buffer, tmp->nick, sizeof buffer);
		my_strmcat(buffer, " ", sizeof buffer);
		buffer_len += len + 1;
	}
	malloc_strcpy(&nicks, buffer);
	say("\t%s +%s (%s): %s", chan->channel, s, get_server_name(chan->server), nicks);
	new_free(&nicks);
}
