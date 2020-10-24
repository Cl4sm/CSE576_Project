void
funny_mode(from, ArgList)
	u_char	*from,
		**ArgList;
{
	u_char	*mode, *channel;
	ChannelList	*tmp = (ChannelList *) 0;

	if (!ArgList[0])
		return;
	if (get_server_version(parsing_server_index) < Server2_6)
	{
		channel = (u_char *) 0;
		mode = ArgList[0];
		PasteArgs(ArgList, 0);
	}
	else
	{
		channel = ArgList[0];
		mode = ArgList[1];
		PasteArgs(ArgList, 1);
	}
	/* if (ignore_mode) */
	if (channel && (tmp = lookup_channel(channel, parsing_server_index, CHAN_NOUNLINK)) && !((tmp->status & CHAN_NAMES) && (tmp->status & CHAN_MODE)))
	{
		update_channel_mode(channel, parsing_server_index, mode);
		tmp->status |= CHAN_MODE;
		update_all_status();
	}
	else
	{
		save_message_from();
		message_from(channel, LOG_CRAP);
		if (channel)
		{
			if (do_hook(current_numeric, "%s %s %s", from,
					channel, mode))
				put_it("%s Mode for channel %s is \"%s\"",
					numeric_banner(), channel, mode);
		}
		else
		{
			if (do_hook(current_numeric, "%s %s", from, mode))
				put_it("%s Channel mode is \"%s\"",
					numeric_banner(), mode);
		}
		restore_message_from();
	}
}
