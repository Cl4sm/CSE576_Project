irc2_parse_server(line)
	u_char	*line;
{
	u_char	*from,
		*comm,
		*end,
		*copy = (u_char *) 0;
	int	numeric;
	u_char	**ArgList;
	u_char	*TrueArgs[MAXPARA + 1];

	if ((u_char *) 0 == line)
		return;

	end = my_strlen(line) + line;
	if (*--end == '\n')
		*end-- = '\0';
	if (*end == '\r')
		*end-- = '\0';

	if (*line == ':')
	{
		if (!do_hook(RAW_IRC_LIST, "%s", line + 1))
			return;
	}
	else if (!do_hook(RAW_IRC_LIST, "%s %s", "*", line))
		return;

	malloc_strcpy(&copy, line);
	ArgList = TrueArgs;
	BreakArgs(copy, &from, ArgList);

	if (!(comm = (*ArgList++)))
		return;		/* Empty line from server - ByeBye */

	/*
	 * XXX!!!
	 * this should fail on '1xxx'!!!
	 */
	if (0 != (numeric = my_atoi(comm)))
		numbered_command(from, numeric, ArgList);
	else if (my_strcmp(comm, "NAMREPLY") == 0)
		funny_namreply(from, ArgList);
	else if (my_strcmp(comm, "WHOREPLY") == 0)
		whoreply(from, ArgList);
	else if (my_strcmp(comm, "NOTICE") == 0)
		parse_notice(from, ArgList);
	/* everything else is handled locally */
	else if (my_strcmp(comm, "PRIVMSG") == 0)
		p_privmsg(from, ArgList);
	else if (my_strcmp(comm, "JOIN") == 0)
		p_channel(from, ArgList);
	else if (my_strcmp(comm, "PART") == 0)
		p_part(from, ArgList);
	else if (my_strcmp(comm, "CHANNEL") == 0)
		p_channel(from, ArgList);
	else if (my_strcmp(comm, "QUIT") == 0)
		p_quit(from, ArgList);
	else if (my_strcmp(comm, "WALL") == 0)
		p_wall(from, ArgList);
	else if (my_strcmp(comm, "WALLOPS") == 0)
		p_wallops(from, ArgList);
	else if (my_strcmp(comm, "LINREPLY") == 0)
		p_linreply(ArgList);
	else if (my_strcmp(comm, "PING") == 0)
		p_ping(ArgList);
	else if (my_strcmp(comm, "TOPIC") == 0)
		p_topic(from, ArgList);
	else if (my_strcmp(comm, "PONG") == 0)
		p_pong(from, ArgList);
	else if (my_strcmp(comm, "INVITE") == 0)
		p_invite(from, ArgList);
	else if (my_strcmp(comm, "NICK") == 0)
		p_nick(from, ArgList);
	else if (my_strcmp(comm, "KILL") == 0)
		p_server_kill(from, ArgList);
	else if (my_strcmp(comm, "MODE") == 0)
		p_mode(from, ArgList);
	else if (my_strcmp(comm, "KICK") == 0)
		p_kick(from, ArgList);
	else if (my_strcmp(comm, "ERROR") == 0)
		p_error(from, ArgList);
	else if (my_strcmp(comm, "ERROR:") == 0) /* Server bug makes this a must */
		p_error(from, ArgList);
	else
	{
		PasteArgs(ArgList, 0);
		if (from)
			say("Odd server stuff: \"%s %s\" (%s)", comm,
				ArgList[0], from);
		else
			say("Odd server stuff: \"%s %s\"", comm, ArgList[0]);
	}
	new_free(&copy);
}
