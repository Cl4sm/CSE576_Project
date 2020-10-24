void
whowas_name(from, ArgList)
	u_char	*from;
	u_char	**ArgList;
{
	u_char	*nick,
		*user,
		*host,
		*channel,
		*ptr,
		*name;
	WhoisStuff *whois_stuff;
	int	lastlog_level;

	PasteArgs(ArgList, 4);
	nick = ArgList[0];
	user = ArgList[1];
	host = ArgList[2];
	channel = ArgList[3];
	name = ArgList[4];
	if (!nick || !user || !host || !channel || !name)
		return;

	lastlog_level = set_lastlog_msg_level(LOG_CRAP);
	whois_stuff = get_server_whois_stuff(parsing_server_index);
	if ((ptr = whois_queue_head(parsing_server_index))
	 && (whois_type_head(parsing_server_index) & (WHOIS_WHOIS|WHOIS_ISON2))
	 && (my_stricmp(ptr, nick) == 0))
	{
		malloc_strcpy(&whois_stuff->nick, nick);
		malloc_strcpy(&whois_stuff->user, user);
		malloc_strcpy(&whois_stuff->host, host);
		malloc_strcpy(&whois_stuff->name, name);
		malloc_strcpy(&whois_stuff->channel, channel);
		new_free(&whois_stuff->away);
		whois_stuff->oper = 0;
		whois_stuff->chop = 0;
		whois_stuff->not_on = 1;
		ignore_whois_crap = 1;
	}
	else
	{
		ignore_whois_crap = 0;
		if (do_hook(current_numeric, "%s %s %s %s %s %s", from, nick,
				user, host, channel, name))
			put_it("%s %s was %s@%s (%s) on channel %s",
				numeric_banner(), nick, user, host, name,
				(*channel == '*') ? (u_char *) "*private*" : channel);
	}
	set_lastlog_msg_level(lastlog_level);
}
