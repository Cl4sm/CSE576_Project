p_channel(from, ArgList)
	u_char	*from;
	u_char	**ArgList;
{
	int	join;
	u_char	*channel;
	int	flag;
	u_char	*s, *ov = NULL;
	int	chan_oper = 0, chan_voice = 0;

	if (!from)
		return;
	flag = double_ignore(from, FromUserHost, IGNORE_CRAP);
	if (my_strcmp(ArgList[0], zero))
	{
		join = 1;
		channel = ArgList[0];
		/*
		 * this \007 should be \a but a lot of compilers are
		 * broken.  *sigh*  -mrg
		 */
		if ((ov = s = my_index(channel, '\007')))
		{
			*s = '\0';
			ov++;
			while (*++s)
			{
				if (*s == 'o')
					chan_oper = 1;
				if (*s == 'v')
					chan_voice = 1;

			}
		}
		malloc_strcpy(&joined_nick, from);
	}
	else
	{
		channel = zero;
		join = 0;
	}
	if (!my_stricmp(from, get_server_nickname(parsing_server_index)))
	{
		if (join)
		{
			add_channel(channel, 0, parsing_server_index, CHAN_JOINED, (ChannelList *) 0);
			if (get_server_version(parsing_server_index) == Server2_5)
				send_to_server("NAMES %s", channel);
			send_to_server("MODE %s", channel);
		}
		else
			remove_channel(channel, parsing_server_index);
	}
	else
	{
		save_message_from();
		message_from(channel, LOG_CRAP);
		if (join)
			add_to_channel(channel, from, parsing_server_index, chan_oper, chan_voice);
		else
			remove_from_channel(channel, from, parsing_server_index);
		restore_message_from();
	}
	if (join)
	{
		if (!get_channel_oper(channel, parsing_server_index))
			in_on_who = 1;
		save_message_from();
		message_from(channel, LOG_CRAP);
		if (flag != IGNORED && do_hook(JOIN_LIST, "%s %s %s", from,
						channel, ov ? ov : (u_char *) ""))
		{
			if (FromUserHost)
				if (ov && *ov)
					say("%s (%s) has joined channel %s +%s", from,
				    FromUserHost, channel, ov);
				else
					say("%s (%s) has joined channel %s", from,
				    FromUserHost, channel);
			else
				if (ov && *ov)
					say("%s has joined channel %s +%s", from, 
					channel, ov);
				else
					say("%s has joined channel %s", from, channel);
		}
		restore_message_from();
		in_on_who = 0;
	}
}
