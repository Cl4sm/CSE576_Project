whois_name(from, ArgList)
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

	PasteArgs(ArgList, 4);
	nick = ArgList[0];
	user = ArgList[1];
	host = ArgList[2];
	channel = ArgList[3];
	name = ArgList[4];
	if (!nick || !user || !host || !channel || !name)
		return;
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
		whois_stuff->not_on = 0;
		ignore_whois_crap = 1;
		eat_away = 1;
	}
	else
	{
		ignore_whois_crap = 0;
		eat_away = 0;
		if (do_hook(current_numeric, "%s %s %s %s %s %s", from, nick,
				user, host, channel, name))
			put_it("%s %s is %s@%s (%s)", numeric_banner(), nick,
					user, host, name);
	}
}
