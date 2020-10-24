static	void
who(command, args, subargs)
	u_char	*command,
		*args,
		*subargs;
{
	u_char	*arg,
		*channel = NULL;
	int	no_args = 1;
	size_t	len;

	if (get_server_version(from_server) == ServerICB)
	{
		icb_put_who(args);
		return;
	}

	who_mask = 0;
	new_free(&who_name);
	new_free(&who_host);
	new_free(&who_server);
	new_free(&who_file);
	new_free(&who_nick);
	new_free(&who_real);
	while ((arg = next_arg(args, &args)) != NULL)
	{
		no_args = 0;
		if ((*arg == '-') && (!isdigit(*(arg + 1))))
		{
			u_char	*cmd = NULL;

			arg++;
			if ((len = my_strlen(arg)) == 0)
			{
				say("Unknown or missing flag");
				return;
			}
			malloc_strcpy(&cmd, arg);
			lower(cmd);
			if (my_strncmp(cmd, "operators", len) == 0)
				who_mask |= WHO_OPS;
			else if (my_strncmp(cmd, "lusers", len) == 0)
				who_mask |= WHO_LUSERS;
			else if (my_strncmp(cmd, "chops", len) == 0)
				who_mask |= WHO_CHOPS;
			else if (my_strncmp(cmd, "hosts", len) == 0)
			{
				if ((arg = next_arg(args, &args)) != NULL)
				{
					who_mask |= WHO_HOST;
					malloc_strcpy(&who_host, arg);
					channel = who_host;
				}
				else
				{
					say("WHO -HOSTS: missing argument");
					new_free(&cmd);
					return;
				}
			}
			else if (my_strncmp(cmd, "here", len) ==0)
				who_mask |= WHO_HERE;
			else if (my_strncmp(cmd, "away", len) ==0)
				who_mask |= WHO_AWAY;
			else if (my_strncmp(cmd, "servers", len) == 0)
			{
				if ((arg = next_arg(args, &args)) != NULL)
				{
					who_mask |= WHO_SERVER;
					malloc_strcpy(&who_server, arg);
					channel = who_server;
				}
				else
				{
					say("WHO -SERVERS: missing argument");
					new_free(&cmd);
					return;
				}
			}
			else if (my_strncmp(cmd, "name", len) == 0)
			{
				if ((arg = next_arg(args, &args)) != NULL)
				{
					who_mask |= WHO_NAME;
					malloc_strcpy(&who_name, arg);
					channel = who_name;
				}
				else
				{
					say("WHO -NAME: missing argument");
					new_free(&cmd);
					return;
				}
			}
			else if (my_strncmp(cmd, "realname", len) == 0)
			{
				if ((arg = next_arg(args, &args)) != NULL)
				{
					who_mask |= WHO_REAL;
					malloc_strcpy(&who_real, arg);
					channel = who_real;
				}
				else
				{
					say("WHO -REALNAME: missing argument");
					new_free(&cmd);
					return;
				}
			}
			else if (my_strncmp(cmd, "nick", len) == 0)
			{
				if ((arg = next_arg(args, &args)) != NULL)
				{
					who_mask |= WHO_NICK;
					malloc_strcpy(&who_nick, arg);
					channel = who_nick;
				}
				else
				{
					say("WHO -NICK: missing argument");
					new_free(&cmd);
					return;
				}
				/* WHO -FILE by Martin 'Efchen' Friedrich */
			}
			else if (my_strncmp(cmd, "file", len) == 0)
			{
				who_mask |= WHO_FILE;
				if ((arg = next_arg(args, &args)) != NULL)
				{
					malloc_strcpy(&who_file, arg);
				}
				else
				{
					say("WHO -FILE: missing argument");
					new_free(&cmd);
					return;
				}
			}
			else
			{
				say("Unknown or missing flag");
				new_free(&cmd);
				return;
			}
			new_free(&cmd);
		}
		else if (my_strcmp(arg, "*") == 0)
		{
			channel = get_channel_by_refnum(0);
			if (!channel || *channel == '0')

			{
				say("I wouldn't do that if I were you");
				return;
			}
		}
		else
			channel = arg;
	}
	if (no_args)
		say("No argument specified");
	else
	{
		u_char *o = NULL;

		if (who_mask & WHO_OPS)
			o = UP(" o");
		if (!channel && who_mask & WHO_OPS)
			channel = UP("*");
		send_to_server("%s %s%s", command,
				channel ? channel : empty_string,
				o ? o : empty_string);
	}
}
