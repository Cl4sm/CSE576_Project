end_of_whois(from, ArgList)
	u_char	*from;
	u_char	**ArgList;
{
	u_char	*nick;
	u_char	*ptr;
	WhoisStuff *whois_stuff;

	whois_stuff = get_server_whois_stuff(parsing_server_index);

	show_away_flag = 0;
	set_server_whois(parsing_server_index,1);
	if ((nick = ArgList[0]) != NULL)
	{
		ptr = whois_queue_head(parsing_server_index);
		if (ptr && (whois_type_head(parsing_server_index) & (WHOIS_WHOIS|WHOIS_ISON2)) && (my_stricmp(ptr, nick) == 0))
		{
			WhoisQueue *thing;

			thing = remove_from_whois_queue(parsing_server_index);
			whois_stuff->not_on = 0;
			thing->func(whois_stuff, thing->nick, thing->text);
			new_free(&whois_stuff->channels);
			new_free(&thing->nick);
			new_free(&thing->text);
			new_free(&thing);
			ignore_whois_crap = 0;
			return;
		}
		PasteArgs(ArgList, 0);
		if (do_hook(current_numeric, "%s %s", from, ArgList[0]))
			if (get_int_var(SHOW_END_OF_MSGS_VAR))
				display_msg(from, ArgList);
	}
}
