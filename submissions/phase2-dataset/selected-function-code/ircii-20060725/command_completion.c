void
command_completion(key, ptr)
	u_int	key;
	u_char	*ptr;
{
	int	do_aliases;
	int	cmd_cnt,
		alias_cnt,
		i,
		c,
		len;
	u_char	**aliases = NULL;
	u_char	*line = NULL,
		*com,
		*cmdchars,
		*rest,
		firstcmdchar = '/';
	u_char	buffer[BIG_BUFFER_SIZE];
	IrcCommand	*command;

	malloc_strcpy(&line, get_input());
	if ((com = next_arg(line, &rest)) != NULL)
	{
		if (!(cmdchars = get_string_var(CMDCHARS_VAR)))
			cmdchars = UP(DEFAULT_CMDCHARS);
		if (my_index(cmdchars, *com))
		{
			firstcmdchar = *cmdchars;
			com++;
			if (*com && my_index(cmdchars, *com))
			{
				do_aliases = 0;
				alias_cnt = 0;
				com++;
			}
			else
				do_aliases = 1;
			upper(com);
			if (do_aliases)
				aliases = match_alias(com, &alias_cnt,
					COMMAND_ALIAS);
			if ((command = find_command(com, &cmd_cnt)) != NULL)
			{
				if (cmd_cnt < 0)
					cmd_cnt *= -1;
				/* special case for the empty string */

				if (*(command[0].name) == (u_char) 0)
				{
					command++;
					cmd_cnt = NUMBER_OF_COMMANDS;
				}
			}
			if ((alias_cnt == 1) && (cmd_cnt == 0))
			{
				snprintf(CP(buffer), sizeof buffer, "%c%s %s", firstcmdchar,
					aliases[0], rest);
				set_input(buffer);
				new_free(&(aliases[0]));
				new_free(&aliases);
				update_input(UPDATE_ALL);
			}
			else if (((cmd_cnt == 1) && (alias_cnt == 0)) ||
			    ((cmd_cnt == 1) && (alias_cnt == 1) &&
			    (my_strcmp(aliases[0], command[0].name) == 0)))
			{
				snprintf(CP(buffer), sizeof buffer, "%c%s%s %s", firstcmdchar,
					do_aliases ? (u_char *) "" : &firstcmdchar,
					command[0].name, rest);
				set_input(buffer);
				update_input(UPDATE_ALL);
			}
			else
			{
				*buffer = (u_char) 0;
				if (command)
				{
					say("Commands:");
					my_strmcpy(buffer, "\t", sizeof buffer);
					c = 0;
					for (i = 0; i < cmd_cnt; i++)
					{
						my_strmcat(buffer, command[i].name,
							sizeof buffer);
						for (len =
						    my_strlen(command[i].name);
						    len < 15; len++)
							my_strmcat(buffer, " ",
							    sizeof buffer);
						if (++c == 4)
						{
							say("%s", buffer);
							my_strmcpy(buffer, "\t",
							    sizeof buffer);
							c = 0;
						}
					}
					if (c)
						say("%s", buffer);
				}
				if (aliases)
				{
					say("Aliases:");
					my_strmcpy(buffer, "\t", sizeof buffer);
					c = 0;
					for (i = 0; i < alias_cnt; i++)
					{
						my_strmcat(buffer, aliases[i],
							sizeof buffer);
						for (len = my_strlen(aliases[i]);
								len < 15; len++)
							my_strmcat(buffer, " ",
							    sizeof buffer);
						if (++c == 4)
						{
							say("%s", buffer);
							my_strmcpy(buffer, "\t",
							    sizeof buffer);
							c = 0;
						}
						new_free(&(aliases[i]));
					}
					if ((int) my_strlen(buffer) > 1)
						say("%s", buffer);
					new_free(&aliases);
				}
				if (!*buffer)
					term_beep();
			}
		}
		else
			term_beep();
	}
	else
		term_beep();
	new_free(&line);
}
