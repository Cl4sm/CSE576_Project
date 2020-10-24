static	void
p_part(from, ArgList)
	u_char	*from,
		**ArgList;
{
	u_char	*channel;
	u_char	*comment;
	int	flag;

	if (!from)
		return;
	flag = double_ignore(from, FromUserHost, IGNORE_CRAP);
	channel = ArgList[0];
	if (!is_on_channel(channel, parsing_server_index, from))
		return;
	comment = ArgList[1];
	if (!comment)
		comment = empty_string;
	in_on_who = 1;
	if (flag != IGNORED)
	{
		save_message_from();
		message_from(channel, LOG_CRAP);
		if (do_hook(LEAVE_LIST, "%s %s %s", from, channel, comment))
		{
			if (comment && *comment != '\0')
				say("%s has left channel %s (%s)", from, channel, comment);
			else
				say("%s has left channel %s", from, channel);
		}
		restore_message_from();
	}
	if (my_stricmp(from, get_server_nickname(parsing_server_index)) == 0)
		remove_channel(channel, parsing_server_index);
	else
		remove_from_channel(channel, from, parsing_server_index);
	in_on_who = 0;
}
