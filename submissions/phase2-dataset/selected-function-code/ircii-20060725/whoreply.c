void
whoreply(from, ArgList)
	u_char	**ArgList,
		*from;
{
	static	u_char	format[40];
	static	int	last_width = -1;
	int	ok = 1;
	u_char	*channel,
		*user,
		*host,
		*server,
		*nick,
		*status,
		*name;
	int	i;

	FILE	*fip;
	u_char	buf_data[BUFSIZ];

	if (last_width != get_int_var(CHANNEL_NAME_WIDTH_VAR))
	{
		if ((last_width = get_int_var(CHANNEL_NAME_WIDTH_VAR)) != 0)
		    snprintf(CP(format), sizeof format, "%%-%u.%us %%-9s %%-3s %%s@%%s (%%s)",
					(u_char) last_width,
					(u_char) last_width);
		else
		    my_strcpy(format, "%s\t%-9s %-3s %s@%s (%s)");
	}
	i = 0;
	channel = user = host = server = nick = status = name = empty_string;
	if (ArgList[i])
		channel = ArgList[i++];
	if (ArgList[i])
		user = ArgList[i++];
	if (ArgList[i])
		host = ArgList[i++];
	if (ArgList[i])
		server = ArgList[i++];
	if (ArgList[i])
		nick = ArgList[i++];
	if (ArgList[i])
		status = ArgList[i++];
	PasteArgs(ArgList, i);

	if (*status == 'S')	/* this only true for the header WHOREPLY */
	{
		channel = UP("Channel");
		if (((who_mask & WHO_FILE) == 0) || (fopen (CP(who_file), "r")))
		{
			if (do_hook(WHO_LIST, "%s %s %s %s %s %s", channel,
					nick, status, user, host, ArgList[6]))
				put_it(CP(format), channel, nick, status, user,
					host, ArgList[6]);
			return;
		}
	}

	if (ArgList[i])
		name = ArgList[i];

	if (who_mask)
	{
		if (who_mask & WHO_HERE)
			ok = ok && (*status == 'H');
		if (who_mask & WHO_AWAY)
			ok = ok && (*status == 'G');
		if (who_mask & WHO_OPS)
			ok = ok && (*(status + 1) == '*');
		if (who_mask & WHO_LUSERS)
			ok = ok && (*(status + 1) != '*');
		if (who_mask & WHO_CHOPS)
			ok = ok && ((*(status + 1) == '@') ||
			(*(status + 2) == '@'));
		if (who_mask & WHO_NAME)
			ok = ok && wild_match(who_name, user);
		if (who_mask & WHO_NICK)
			ok = ok && wild_match(who_nick, nick);
		if (who_mask & WHO_HOST)
			ok = ok && wild_match(who_host, host);
		if (who_mask & WHO_REAL)
			ok = ok && wild_match(who_real, name);
		if (who_mask & WHO_SERVER)
			ok = ok && wild_match(who_server, server);
		if (who_mask & WHO_FILE)
		{
			ok = 0;
			cannot_open = (u_char *) 0;
			if ((fip = fopen(CP(who_file), "r")) != (FILE *) 0)
			{
				while (fgets ((char *)buf_data, BUFSIZ, fip) !=
								(char *) 0)
				{
					buf_data[my_strlen(buf_data)-1] = '\0';
					ok = ok || wild_match(buf_data, nick);
				}
				fclose (fip);
			} else
				cannot_open = who_file;
		}
	}
	if (ok)
	{
		if (do_hook(WHO_LIST, "%s %s %s %s %s %s", channel, nick,
				status, user, host, name))
		{
			if (get_int_var(SHOW_WHO_HOPCOUNT_VAR))
				put_it(CP(format), channel, nick, status, user, host,
					name);
			else
			{
				u_char	*tmp;

				if ((tmp = (u_char *) my_index(name, ' ')) !=
								(u_char *) 0)
					tmp++;
				else
					tmp = name;
				put_it(CP(format), channel, nick, status, user, host,
					tmp);
			}
		}
	}
}
