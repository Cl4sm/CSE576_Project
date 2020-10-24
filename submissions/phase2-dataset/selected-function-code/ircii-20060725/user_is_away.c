void
user_is_away(from, ArgList)
	u_char	*from,
		**ArgList;
{
	static	u_char	*last_away_msg = (u_char *) 0,
			*last_away_nick = (u_char *) 0;
	u_char	*message,
		*who;
	WhoisStuff *whois_stuff;

if (!from)
	return;

	PasteArgs(ArgList, 1);
	whois_stuff = get_server_whois_stuff(parsing_server_index);

	if ((who = ArgList[0]) && (message = ArgList[1]))
	{
		if (whois_stuff->nick && (!my_strcmp(who, whois_stuff->nick)) &&
				eat_away)
			malloc_strcpy(&whois_stuff->away, message);
		else
		{
			if (!show_away_flag && get_int_var(SHOW_AWAY_ONCE_VAR))
			{
				if (!last_away_msg || my_strcmp(last_away_nick,
					from) || my_strcmp(last_away_msg, message))
				{
					malloc_strcpy(&last_away_nick, from);
					malloc_strcpy(&last_away_msg, message);
				}
				else return;
			}
			if (do_hook(current_numeric, "%s %s", who, message))
				put_it("%s %s is away: %s",numeric_banner(),
					who, message);
		}
		eat_away = 0;
	}
}
