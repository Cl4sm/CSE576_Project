funny_namreply(from, Args)
	u_char	*from;
	u_char	**Args;
{
	u_char	*type,
		*nick,
		*channel;
	static	u_char	format[40];
	static	int	last_width = -1;
	int	cnt;
	u_char	*ptr;
	u_char	*line;
	ChannelList	*tmp = (ChannelList *) 0;

	PasteArgs(Args, 2);
	type = Args[0];
	channel = Args[1];
	line = Args[2];
	save_message_from();
	message_from(channel, LOG_CRAP);
	if ((tmp = lookup_channel(channel, parsing_server_index, CHAN_NOUNLINK)) && !((tmp->status & CHAN_NAMES) && (tmp->status & CHAN_MODE)))
	{
		if (do_hook(current_numeric, "%s %s %s %s", from, type, channel,
			line) && get_int_var(SHOW_CHANNEL_NAMES_VAR))
			say("Users on %s: %s", channel, line);
		while ((nick = next_arg(line, &line)) != NULL)
			add_to_channel(channel, nick, parsing_server_index, 0, 0);
		tmp->status |= CHAN_NAMES;
		goto out;
	}
	if (last_width != get_int_var(CHANNEL_NAME_WIDTH_VAR))
	{
		if ((last_width = get_int_var(CHANNEL_NAME_WIDTH_VAR)) != 0)
			snprintf(CP(format), sizeof format, "%%s: %%-%u.%us %%s",
				(u_char) last_width,
				(u_char) last_width);
		else
			my_strmcpy(format, "%s: %s\t%s", sizeof format);
	}
	ptr = line;
	for (cnt = -1; ptr; cnt++)
	{
		if ((ptr = my_index(ptr, ' ')) != NULL)
			ptr++;
	}
	if (funny_min && (cnt < funny_min))
		return;
	else if (funny_max && (cnt > funny_max))
		return;
	if ((funny_flags & FUNNY_PRIVATE) && (*type == '='))
		return;
	if ((funny_flags & FUNNY_PUBLIC) && (*type == '*'))
		return;
	if (type && channel)
	{
		if (match_str)
		{
			if (wild_match(match_str, channel) == 0)
				return;
		}
		if (do_hook(current_numeric, "%s %s %s %s", from, type, channel,
			line) && do_hook(NAMES_LIST, "%s %s", channel, line))
		{
			switch (*type)
			{
			case '=':
				if (last_width && (my_strlen(channel) > last_width))
				{
					channel[last_width-1] = '>';
					channel[last_width] = (u_char) 0;
				}
				put_it(CP(format), "Pub", channel, line);
				break;
			case '*':
				put_it(CP(format), "Prv", channel, line);
				break;
			case '@':
				put_it(CP(format), "Sec", channel, line);
				break;
			}
		}
	}
out:
	restore_message_from();
}
