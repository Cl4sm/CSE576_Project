whois_chop(from, ArgList)
	u_char	*from;
	u_char	**ArgList;
{
	WhoisStuff *whois_stuff;

	whois_stuff = get_server_whois_stuff(parsing_server_index);
	PasteArgs(ArgList, 1);
	if (ignore_whois_crap)
		whois_stuff->chop = 1;
	else
	{
		u_char	*nick;

		if ((nick = ArgList[0]) != NULL)
		{
			if (do_hook(current_numeric, "%s %s %s",from, nick,
					ArgList[1]))
				put_it("%s %s (is a channel operator)",
					numeric_banner(), nick, ArgList[1]);
		}
	}
}
