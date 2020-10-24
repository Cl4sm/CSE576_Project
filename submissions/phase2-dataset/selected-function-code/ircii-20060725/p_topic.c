static	void
p_topic(from, ArgList)
	u_char	*from,
		**ArgList;
{
	int	flag;

	if (!from)
		return;
	flag = double_ignore(from, FromUserHost, IGNORE_CRAP);
	if (flag == IGNORED)
		return;

	save_message_from();
	if (!ArgList[1])
	{
		message_from((u_char *) 0, LOG_CRAP);
		if (do_hook(TOPIC_LIST, "%s * %s", from, ArgList[0]))
			say("%s has changed the topic to %s", from, ArgList[0]);
	}
	else
	{
		message_from(ArgList[0], LOG_CRAP);
		if (do_hook(TOPIC_LIST, "%s %s %s", from, ArgList[0], ArgList[1]))
			say("%s has changed the topic on channel %s to %s",
				from, ArgList[0], ArgList[1]);
	}
	restore_message_from();
}
