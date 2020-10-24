p_quit(from, ArgList)
	u_char	*from,
		**ArgList;
{
	int	one_prints = 0;
	u_char	*chan;
	u_char	*Reason;
	int	flag;

	if (!from)
		return;
	flag = double_ignore(from, FromUserHost, IGNORE_CRAP);
	save_message_from();
	if (flag != IGNORED)
	{
		PasteArgs(ArgList, 0);
		Reason = ArgList[0] ? ArgList[0] : (u_char *) "?";
		for (chan = walk_channels(from, 1, parsing_server_index); chan; chan = walk_channels(from, 0, -1))
		{
			message_from(chan, LOG_CRAP);
			if (do_hook(CHANNEL_SIGNOFF_LIST, "%s %s %s", chan, from, Reason))
				one_prints = 1;
		}
		if (one_prints)
		{
			message_from(what_channel(from, parsing_server_index), LOG_CRAP);
			if (do_hook(SIGNOFF_LIST, "%s %s", from, Reason))
				say("Signoff: %s (%s)", from, Reason);
		}
	}
	message_from((u_char *) 0, LOG_CURRENT);
	remove_from_channel((u_char *) 0, from, parsing_server_index);
	notify_mark(from, 0, 0);
	restore_message_from();
}
