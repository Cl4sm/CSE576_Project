p_mode(from, ArgList)
	u_char	*from,
		**ArgList;
{
	u_char	*channel;
	u_char	*line;
	int	flag;

	if (!from)
		return;
	flag = double_ignore(from, FromUserHost, IGNORE_CRAP);
	PasteArgs(ArgList, 1);
	channel = ArgList[0];
	line = ArgList[1];
	save_message_from();
	message_from(channel, LOG_CRAP);
	if (channel && line)
	{
		if (is_channel(channel))
		{
			if (flag != IGNORED && do_hook(MODE_LIST, "%s %s %s",
					from, channel, line))
				say("Mode change \"%s\" on channel %s by %s",
						line, channel, from);
			update_channel_mode(channel, parsing_server_index, line);
		}
		else
		{
			if (flag != IGNORED && do_hook(MODE_LIST, "%s %s %s",
					from, channel, line))
				say("Mode change \"%s\" for user %s by %s",
						line, channel, from);
			update_user_mode(line);
		}
		update_all_status();
	}
	restore_message_from();
}
