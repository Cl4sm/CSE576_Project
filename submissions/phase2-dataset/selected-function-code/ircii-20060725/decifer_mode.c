decifer_mode(u_char *mode_string, u_long *mode, int *chop, NickList **nicks,
	     u_char **key)
{
	u_char	*limit = 0;
	u_char	*person;
	int	add = 0;
	int	limit_set = 0;
	int	limit_reset = 0;
	u_char	*rest,
		*the_key;
	NickList *ThisNick;
	u_long	value = 0;

	if (!(mode_string = next_arg(mode_string, &rest)))
		return -1;
	for (; *mode_string; mode_string++)
	{
		switch (*mode_string)
		{
		case '+':
			add = 1;
			value = 0;
			break;
		case '-':
			add = 0;
			value = 0;
			break;
		case 'a':
			value = MODE_ANONYMOUS;
			break;
		case 'c':
			value = MODE_COLOURLESS;
			break;
		case 'i':
			value = MODE_INVITE;
			break;
		case 'k':
			value = MODE_KEY;
			the_key = next_arg(rest, &rest);
			if (add)
				malloc_strcpy(key, the_key);
			else
				new_free(key);
			break;	
		case 'l':
			value = MODE_LIMIT;
			if (add)
			{
				limit_set = 1;
				if (!(limit = next_arg(rest, &rest)))
					limit = empty_string;
				else if (0 == my_strncmp(limit, zero, 1))
					limit_reset = 1, limit_set = 0, add = 0;
			}
			else
				limit_reset = 1;
			break;
		case 'm':
			value = MODE_MODERATED;
			break;
		case 'o':
			if ((person = next_arg(rest, &rest)) && !my_stricmp(person, get_server_nickname(from_server))) {
				if (add)
					*chop |= CHAN_CHOP;
				else
					*chop &= ~CHAN_CHOP;
			}
			ThisNick = (NickList *) list_lookup((List **)(void *)nicks, person, !USE_WILDCARDS, !REMOVE_FROM_LIST);
			if (ThisNick)
				ThisNick->chanop = add;
			break;
		case 'n':
			value = MODE_MSGS;
			break;
		case 'p':
			value = MODE_PRIVATE;
			break;
		case 'q':
			value = MODE_QUIET;
			break;
		case 'r':
			value = MODE_REOP;
			break;
		case 's':
			value = MODE_SECRET;
			break;
		case 't':
			value = MODE_TOPIC;
			break;
		case 'v':
			person = next_arg(rest, &rest);
			ThisNick = (NickList *) list_lookup((List **) nicks, person, !USE_WILDCARDS, !REMOVE_FROM_LIST);
			if (ThisNick)
				ThisNick->hasvoice = add;
			break;
		case 'b':
		case 'e':
		case 'I':
		case 'O': /* this is a weird special case */
			(void) next_arg(rest, &rest);
			break;
		case 'R':
			value = MODE_REGONLY;
			break;
		}
		if (add)
			*mode |= value;
		else
			*mode &= ~value;
	}
	if (limit_set)
		return (my_atoi(limit));
	else if (limit_reset)
		return(0);
	else
		return(-1);
}
