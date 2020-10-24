channel_topic(from, ArgList)
	u_char	*from,
		**ArgList;
{
	u_char	*topic, *channel;

	save_message_from();
	if (ArgList[1] && is_channel(ArgList[0]))
	{
		topic = ArgList[1];
		channel = ArgList[0];
		message_from(channel, LOG_CRAP);
		put_it("%s Topic for %s: %s", numeric_banner(), channel,
			topic);
	}
	else
	{
		message_from((u_char *) 0, LOG_CURRENT);
		PasteArgs(ArgList, 0);
		put_it("%s Topic: %s", numeric_banner(), ArgList[0]);
	}
	restore_message_from();
}
