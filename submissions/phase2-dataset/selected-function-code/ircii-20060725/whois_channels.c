whois_channels(from, ArgList)
	u_char	*from;
	u_char	**ArgList;
{
	u_char	*ptr;
	u_char	*line;
	WhoisStuff *whois_stuff;

	PasteArgs(ArgList, 1);
	line = ArgList[1];
	whois_stuff = get_server_whois_stuff(parsing_server_index);
	if ((ptr = whois_queue_head(parsing_server_index))
	 && (whois_type_head(parsing_server_index) & (WHOIS_WHOIS|WHOIS_ISON2))
	 && whois_stuff->nick
	 && (my_stricmp(ptr, whois_stuff->nick) == 0))
	{
		if (whois_stuff->channels == (u_char *) 0)
			malloc_strcpy(&whois_stuff->channels, line);
		else
			malloc_strcat(&whois_stuff->channels, line);
	}
	else
	{
		if (do_hook(current_numeric, "%s %s", from, line))
			put_it("%s on channels: %s", numeric_banner(), line);
	}
}
