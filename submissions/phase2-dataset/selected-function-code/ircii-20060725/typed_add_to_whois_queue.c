typed_add_to_whois_queue(int type, u_char *nick, void (*func)(WhoisStuff *, u_char *, u_char *), char *format, va_list vlist)
{
	u_char	lbuf[BIG_BUFFER_SIZE];
	WhoisQueue *new;
	u_char	*p = nick;

	if (get_server_version(from_server) == ServerICB)
	{
#if 0
		yell("--- typed_add_to_whois_queue: panic?  attempted on ICB server..");
#endif
		return;
	}
	if ((nick == (u_char *) 0) || ! is_server_connected(from_server) ||
	    (server_list[from_server].write == -1)) /* XXX STOP-GAP -Sol */
		return;

	for (; *p == ' ' || *p == '\t'; p++);
	if (!*p)
		return;	/* nick should always be a non-blank string, but
			   I'd rather check because of a "ISON not enough
			   parameters" coming from the server -Sol */

	if (my_index(nick, '*') == (u_char *) 0)
	{
		new = (WhoisQueue *) new_malloc(sizeof(WhoisQueue));
		new->text = (u_char *) 0;
		new->nick = (u_char *) 0;
		new->func = func;
		new->next = (WhoisQueue *) 0;
		new->type = type;
		if (format)
		{
			vsnprintf(CP(lbuf), sizeof lbuf, format, vlist);
			malloc_strcpy(&(new->text), lbuf);
		}
		malloc_strcpy(&(new->nick), nick);
		if ((void *) get_server_qhead(from_server) == (void *) 0)
			set_server_qhead(from_server, new);
		if (get_server_qtail(from_server))
			((WhoisQueue *) get_server_qtail(from_server))->next = new;
		set_server_qtail(from_server, new);
		switch(type)
		{
		case WHOIS_ISON:
#ifdef MONITOR_Q
			put_it("+++ ISON %s", nick);
#endif /* MONITOR_Q */
			send_to_server("ISON %s", nick);
			break;
		case WHOIS_USERHOST:
#ifdef MONITOR_Q
			put_it("+++ USERHOST %s", nick);
#endif /* MONITOR_Q */
			send_to_server("USERHOST %s", nick);
			break;
		case WHOIS_WHOIS:
		case WHOIS_ISON2:
#ifdef MONITOR_Q
			put_it("+++ WHOIS %s", nick);
#endif /* MONITOR_Q */
			send_to_server("WHOIS %s", nick);
			if (!get_server_whois(from_server))
				send_to_server("WHOIS !%s", nick);
				/* postfix with nick so we know who we're
				   talking about -Sol */
				/* send "WHOIS !nick" and expect
				   "!nick: No such nick/channel" :
				   it means the real query was completed
				   and the dummy query is to be ignored
				   in no_such_nickname() -Sol */
			break;
		}
	}
}
