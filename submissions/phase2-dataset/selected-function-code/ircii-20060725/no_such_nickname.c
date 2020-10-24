no_such_nickname(from, ArgList)
	u_char	*from,
		**ArgList;
{
	u_char	*nick;
	u_char		*ptr;
	WhoisStuff	*whois_stuff;

	whois_stuff = get_server_whois_stuff(parsing_server_index);
	ptr = whois_queue_head(parsing_server_index);
	PasteArgs(ArgList, 1);
	nick = ArgList[0];
	if (*nick == '!')
	{
		u_char	*name = nick+1;

		if (ptr && (whois_type_head(parsing_server_index) & (WHOIS_WHOIS|WHOIS_ISON2)) && !my_strcmp(ptr, name))
		{
			WhoisQueue *thing;

			/* There's a query in the WhoisQueue : assume it's
			   completed and remove it from the queue -Sol */

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
		return;
	}
	notify_mark(nick, 0, 0);
	if (ptr && (whois_type_head(parsing_server_index) == WHOIS_ISON2) &&
	    !my_strcmp(ptr, nick))
	{
		WhoisQueue *thing;

		/* Remove query from queue. Don't display anything. -Sol */

		thing = remove_from_whois_queue(parsing_server_index);
		new_free(&whois_stuff->channels);
		new_free(&thing->nick);
		new_free(&thing->text);
		new_free(&thing);
		ignore_whois_crap = 0;
		return;
	}
	if (do_hook(current_numeric, "%s %s %s", from, nick, ArgList[1]))
		put_it("%s %s: %s", numeric_banner(), nick, ArgList[1]);
	if ((get_server_version(parsing_server_index) > Server2_5) &&
	    get_int_var(AUTO_WHOWAS_VAR))
		send_to_server("WHOWAS %s", nick);
}
