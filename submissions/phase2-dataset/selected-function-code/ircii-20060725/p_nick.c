static	void
p_nick(from, ArgList)
	u_char	*from,
		**ArgList;
{
	int	one_prints = 0,
		its_me = 0;
	u_char	*chan;
	u_char	*line;
	int	flag;

	if (!from)
		return;
	flag = double_ignore(from, FromUserHost, IGNORE_CRAP);
	line = ArgList[0];
	if (my_stricmp(from, get_server_nickname(parsing_server_index)) == 0){
		if (parsing_server_index == primary_server)
			malloc_strcpy(&nickname, line);
		set_server_nickname(parsing_server_index, line);
		its_me = 1;
	}
	save_message_from();
	if (flag != IGNORED)
	{
		for (chan = walk_channels(from, 1, parsing_server_index); chan;
				chan = walk_channels(from, 0, -1))
		{
			message_from(chan, LOG_CRAP);
			if (do_hook(CHANNEL_NICK_LIST, "%s %s %s", chan, from, line))
				one_prints = 1;
		}
		if (one_prints)
		{
			if (its_me)
				message_from((u_char *) 0, LOG_CRAP);
			else
				message_from(what_channel(from, parsing_server_index), LOG_CRAP);
			if (do_hook(NICKNAME_LIST, "%s %s", from, line))
				say("%s is now known as %s", from, line);
		}
	}
	rename_nick(from, line, parsing_server_index);
	if (my_stricmp(from, line))
	{
		message_from((u_char *) 0, LOG_CURRENT);
		notify_mark(from, 0, 0);
		notify_mark(line, 1, 0);
	}
	restore_message_from();
}
