static	void
funny_stuff(command, args, subargs)
	u_char	*command,
		*args,
		*subargs;
{
	u_char	*arg,
		*cmd = (u_char *) 0,
		*stuff,
		*s;
	int	min = 0,
		max = 0,
		flags = 0;
	size_t	len;

	if (get_server_version(from_server) == ServerICB)
	{
		icb_put_funny_stuff(command, args, subargs);
		return;
	}
	stuff = empty_string;
	while ((arg = next_arg(args, &args)) != NULL)
	{
		len = my_strlen(arg);
		malloc_strcpy(&cmd, arg);
		upper(cmd);
		if (my_strncmp(cmd, "-MAX", len) == 0)
		{
			if ((arg = next_arg(args, &args)) != NULL)
				max = my_atoi(arg);
		}
		else if (my_strncmp(cmd, "-MIN", len) == 0)
		{
			if ((arg = next_arg(args, &args)) != NULL)
				min = my_atoi(arg);
		}
		else if (my_strncmp(cmd, "-ALL", len) == 0)
		{
			flags &= ~(FUNNY_PUBLIC | FUNNY_PRIVATE);
		}
		else if (my_strncmp(cmd, "-PUBLIC", len) == 0)
		{
			flags |= FUNNY_PUBLIC;
			flags &= ~FUNNY_PRIVATE;
		}
		else if (my_strncmp(cmd, "-PRIVATE", len) == 0)
		{
			flags |= FUNNY_PRIVATE;
			flags &= ~FUNNY_PUBLIC;
		}
		else if (my_strncmp(cmd, "-TOPIC", len) == 0)
			flags |= FUNNY_TOPIC;
		else if (my_strncmp(cmd, "-WIDE", len) == 0)
			flags |= FUNNY_WIDE;
		else if (my_strncmp(cmd, "-USERS", len) == 0)
			flags |= FUNNY_USERS;
		else if (my_strncmp(cmd, "-NAME", len) == 0)
			flags |= FUNNY_NAME;
		else
			stuff = arg;
		new_free(&cmd);
	}
	set_funny_flags(min, max, flags);
	if (my_strcmp(stuff, "*") == 0)
		if (!(stuff = get_channel_by_refnum(0)))
			stuff = empty_string;
	if ((s = my_index(stuff, '*')) &&
	    !is_on_channel(stuff, from_server, get_server_nickname(from_server)) &&
	    !(s > stuff && s[-1] == ':'))
	{
		funny_match(stuff);
		send_to_server("%s %s", command, empty_string);
	}
	else
	{
		funny_match(NULL);
		send_to_server("%s %s", command, stuff);
	}
}
