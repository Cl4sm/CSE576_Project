static	void
userhost(command, args, subargs)
	u_char	*command,
		*args,
		*subargs;
{
	int	n = 0,
		total = 0,
		userhost_cmd = 0;
	u_char	*nick;
	u_char	buffer[BIG_BUFFER_SIZE];

	while ((nick = next_arg(args, &args)) != NULL)
	{
		size_t	len;

		++total;
		len = my_strlen(nick);
		if (!my_strnicmp(nick, UP("-CMD"), len))
		{
			if (total < 2)
			{
				yell("userhost -cmd with no nick!");
				return;
			}
			userhost_cmd = 1;
			break;
		}
		else
		{
			if (n++)
				my_strmcat(buffer, " ", sizeof buffer);
			else
				*buffer = '\0';
			my_strmcat(buffer, nick, sizeof buffer);
		}
	}
	if (n)
	{
		u_char	*the_list = (u_char *) 0;
		u_char	*s, *t;
		int	i;

		malloc_strcpy(&the_list, buffer);
		s = t = the_list;
		while (n)
		{
			for (i = 5; i && *s; s++)
				if (' ' == *s)
					i--, n--;
			if (' ' == *(s - 1))
				*(s - 1) = '\0';
			else
				n--;
			my_strmcpy(buffer, t, sizeof buffer);
			t = s;

			if (userhost_cmd)
				add_to_whois_queue(buffer, userhost_cmd_returned, "%s", args);
			else
				add_to_whois_queue(buffer, USERHOST_USERHOST, 0);
		}
		new_free(&the_list);
	}
	else if (!total)
		/* Default to yourself.  */
		add_to_whois_queue(get_server_nickname(from_server), USERHOST_USERHOST, 0);
}
