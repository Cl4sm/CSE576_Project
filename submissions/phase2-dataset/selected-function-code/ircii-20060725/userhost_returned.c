void
userhost_returned(from, ArgList)
	u_char	*from,
		**ArgList;
{
	WhoisQueue *thing;
	WhoisStuff *whois_stuff = NULL;
	u_char	*nick,
		*cnick = NULL,
		*tnick;
	u_char	*user,
		*cuser = (u_char *) 0;
	u_char	*host,
		*chost = (u_char *) 0;
	int	ishere,
		cishere = 1;
	int	isoper,
		cisoper = 0;
	int	noton,
		isuser,
		parsed;
	u_char	*queue_nicks;

	if (!ArgList[0])
		return;
	if (whois_type_head(parsing_server_index) == WHOIS_USERHOST)
	{
		isuser = (whois_func_head(parsing_server_index) == USERHOST_USERHOST);
		whois_stuff = get_server_whois_stuff(parsing_server_index);
		thing = remove_from_whois_queue(parsing_server_index);
		queue_nicks = thing->nick;
	}
	else
	{
		isuser = 1;
		thing = (WhoisQueue *) 0;
		queue_nicks = (u_char *) 0;
		whois_stuff = NULL;
	}
	parsed = 0;
	while ((parsed || (cnick = next_arg(ArgList[0], ArgList))) ||
			queue_nicks)
	{
		if (queue_nicks)
		{
			tnick = next_arg(queue_nicks, &queue_nicks);
			if (!*queue_nicks)
				queue_nicks = (u_char *) 0;
		}
		else
			tnick = NULL;
		if (cnick && !parsed)
		{
			if (!(cuser = my_index(cnick,'=')))
				break;
			if (*(cuser - 1) == '*')
			{
				*(cuser - 1) = '\0';
				cisoper = 1;
			}
			else
				cisoper = 0;
			*cuser++ = '\0';
			if (*cuser++ == '+')
				cishere = 1;
			else
				cishere = 0;
			if (!(chost = my_index(cuser, '@')))
				break;
			*chost++ = '\0';
			parsed = 1;
		}
		if (!cnick || (tnick && my_stricmp(cnick, tnick)))
		{
			if (tnick)
				nick = tnick;
			else
				nick = cnick;
			user = host = UP("<UNKNOWN>");
			isoper = 0;
			ishere = 1;
			noton = 1;
		}
		else
		{
			nick = cnick;
			user = cuser;
			host = chost;
			isoper = cisoper;
			ishere = cishere;
			noton = parsed = 0;
		}
		if (!isuser)
		{
			malloc_strcpy(&whois_stuff->nick, nick);
			malloc_strcpy(&whois_stuff->user, user);
			malloc_strcpy(&whois_stuff->host, host);
			whois_stuff->oper = isoper;
			whois_stuff->not_on = noton;
			if (!ishere)
				malloc_strcpy(&whois_stuff->away, empty_string);
			else
				new_free(&whois_stuff->away);
			thing->func(whois_stuff, tnick, thing->text);
			new_free(&whois_stuff->away);
		}
		else
		{
			if (do_hook(current_numeric, "%s %s %s %s %s", nick,
			    isoper ? "+" : "-", ishere ? "+" : "-", user, host))
				put_it("%s %s is %s@%s%s%s", numeric_banner(),
					nick, user, host, isoper ?
					(u_char *) " (Is an IRC operator)" : empty_string,
					ishere ? empty_string : (u_char *) " (away)");
		}
	}
	if (thing)
	{
		new_free(&thing->nick);
		new_free(&thing->text);
		new_free(&thing);
	}
}
