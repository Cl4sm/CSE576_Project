save_settings(command, args, subargs)
	u_char	*command,
		*args,
		*subargs;
{
	u_char	buffer[BIG_BUFFER_SIZE];
	u_char	*arg, *temp;
	int	all = 1, save_force = 0;

	save_which = save_do_all = 0;
	while ((arg = next_arg(args, &args)) != NULL)
	{
		if ('-' == *arg)
		{
			u_char	*cmd = NULL;

			all = 0;
			malloc_strcpy(&cmd, arg+1);
			upper(cmd);
			if (0 == my_strncmp("ALIAS", cmd, 5))
				save_which |= SFLAG_ALIAS;
			else if (0 == my_strncmp("ASSIGN", cmd, 6))
				save_which |= SFLAG_ALIAS;
			else if (0 == my_strncmp("BIND", cmd, 4))
				save_which |= SFLAG_BIND;
			else if (0 == my_strncmp("ON", cmd, 2))
				save_which |= SFLAG_ON;
			else if (0 == my_strncmp("SET", cmd, 3))
				save_which |= SFLAG_SET;
			else if (0 == my_strncmp("NOTIFY", cmd, 6))
				save_which |= SFLAG_NOTIFY;
			else if (0 == my_strncmp("DIGRAPH", cmd, 7))
				save_which |= SFLAG_DIGRAPH;
			else if (0 == my_strncmp("ALL", cmd, 3))
				save_do_all = 1;
			else if (0 == my_strncmp("FORCE", cmd, 3))
				save_force = 1;
			else
			{
				say("%s: unknown argument", arg);
				new_free(&cmd);
				return;
			}
			new_free(&cmd);
			continue;
		}
#ifdef DAEMON_UID
		if (getuid() == DAEMON_UID)
		{
			say("You may only use the default value");
			return;
		}
#endif /* DAEMON_UID */
		temp = expand_twiddle(arg);
		if (temp)
		{
			if (ircrc_file)
				new_free(&ircrc_file);
			ircrc_file = temp;
		}
		else
		{
			say("Unknown user");
			return;
		}
	}
	if (all || !save_which)
		save_which = SFLAG_ALIAS | SFLAG_BIND | SFLAG_ON | SFLAG_SET |
			     SFLAG_NOTIFY | SFLAG_DIGRAPH;
	if (dumb || save_force)
		really_save(ircrc_file, UP("y"));
	else
	{
		snprintf(CP(buffer), sizeof buffer, "Really write %s? ", ircrc_file);
		add_wait_prompt(buffer, really_save, ircrc_file,
				WAIT_PROMPT_LINE);
	}
}
