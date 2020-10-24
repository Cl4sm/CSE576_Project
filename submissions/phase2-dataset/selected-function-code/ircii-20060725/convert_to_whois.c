convert_to_whois()
{
	u_char	*NextAsked;
	u_char	*Names;
	WhoisQueue *thing;
	void	(*func)(WhoisStuff *, u_char *, u_char *);

	if (!(whois_type_head(parsing_server_index) & (WHOIS_USERHOST|WHOIS_WHOIS|WHOIS_ISON2)))
	{
		say("Server does not support USERHOST");
		return; /* USERHOST sent interactively. */
	}
	thing = remove_from_whois_queue(parsing_server_index);
	switch(thing->type)
	{
	case WHOIS_ISON:
		Names = thing->nick;
		while ( (NextAsked = next_arg(Names, &Names)))
		{
			if (thing->func == ison_notify)
			{
				func = (void (*)(WhoisStuff *, u_char *, u_char *)) whois_notify;
				add_to_whois_queue(NextAsked, func, "%s", NextAsked);
			}
			else
				say("Server does not support ISON");
		}
		break;
	case WHOIS_USERHOST:
		add_to_whois_queue(thing->nick, thing->func, "%s", thing->text);
		break;
	}
	new_free(&thing->nick);
	new_free(&thing->text);
	new_free(&thing);
}
