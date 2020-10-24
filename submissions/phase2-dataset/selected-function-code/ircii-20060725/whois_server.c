void
whois_server(from, ArgList)
	u_char	*from;
	u_char	**ArgList;
{
	u_char	*server,
		*ptr;
	u_char	*line;
	WhoisStuff *whois_stuff;

	show_away_flag = 1;
	if (!ArgList[0] || !ArgList[1])
		return;
	if (ArgList[2])
	{
		server = ArgList[1];
		line = ArgList[2];
	}
	else
	{
		server = ArgList[0];
		line = ArgList[1];
	}
	whois_stuff = get_server_whois_stuff(parsing_server_index);
	if ((ptr = whois_queue_head(parsing_server_index))
	 && (whois_type_head(parsing_server_index) & (WHOIS_WHOIS|WHOIS_ISON2))
	 && whois_stuff->nick && /* This is *weird* */
	(my_stricmp(ptr, whois_stuff->nick) == 0))
	{
		malloc_strcpy(&whois_stuff->server, server);
		malloc_strcpy(&whois_stuff->server_stuff, line);
	}
	else
	{
		if (do_hook(current_numeric, "%s %s %s", from, server, line))
			put_it("%s on irc via server %s (%s)",
				numeric_banner(), server, line);
	}
}
