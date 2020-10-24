p_kick(from, ArgList)
	u_char	*from,
		**ArgList;
{
	u_char	*channel,
		*who,
		*comment;

	if (!from)
		return;
	channel = ArgList[0];
	who = ArgList[1];
	comment = ArgList[2];

	if (channel && who)
	{
		save_message_from();
		message_from(channel, LOG_CRAP);
		if (my_stricmp(who, get_server_nickname(parsing_server_index)) == 0)
		{
			if (comment && *comment)
			{
				if (do_hook(KICK_LIST, "%s %s %s %s", who,
						from, channel, comment))
					say("You have been kicked off channel %s by %s (%s)",
						channel, from, comment);
			}
			else
			{
				if (do_hook(KICK_LIST, "%s %s %s", who, from,
						channel))
					say("You have been kicked off channel %s by %s",
						channel, from);
			}
			remove_channel(channel, parsing_server_index);
			update_all_status();
		}
		else
		{
			if (comment && *comment)
			{
				if (do_hook(KICK_LIST, "%s %s %s %s", who,
						from, channel, comment))
					say("%s has been kicked off channel %s by %s (%s)",
						who, channel, from, comment);
			}
			else
			{
				if (do_hook(KICK_LIST, "%s %s %s", who, from,
						channel))
					say("%s has been kicked off channel %s by %s",
						who, channel, from);
			}
			remove_from_channel(channel, who, parsing_server_index);
		}
		restore_message_from();
	}
}
