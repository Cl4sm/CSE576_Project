void
parse_command(line, hist_flag, sub_args)
	u_char	*line;
	int	hist_flag;
	u_char	*sub_args;
{
	static	unsigned int	 level = 0;
	unsigned int	display,
			old_display_var;
	u_char	*cmdchars,
		*com,
		*this_cmd = NULL;
	int	args_flag,
		add_to_hist,
		cmdchar_used;

	if (!line || !*line)
		return;
	if (get_int_var(DEBUG_VAR) & DEBUG_COMMANDS)
		yell("Executing [%d] %s", level, line);
	level++;
	if (!(cmdchars = get_string_var(CMDCHARS_VAR)))
		cmdchars = UP(DEFAULT_CMDCHARS);
	malloc_strcpy(&this_cmd, line);
	add_to_hist = 1;
	if (my_index(cmdchars, *line))
	{
		cmdchar_used = 1;
		com = line + 1;
	}
	else
	{
		cmdchar_used = 0;
		com = line;
	}
	/*
	 * always consider input a command unless we are in interactive mode
	 * and command_mode is off.   -lynx
	 */
	if (hist_flag && !cmdchar_used && !get_int_var(COMMAND_MODE_VAR))
	{
		do_send_text(NULL, line, empty_string);
		if (hist_flag && add_to_hist)
		{
			add_to_history(this_cmd);
			set_input(empty_string);
		}
		/* Special handling for ' and : */
	}
	else if (*com == '\'' && get_int_var(COMMAND_MODE_VAR))
	{
		do_send_text(NULL, line+1, empty_string);
		if (hist_flag && add_to_hist)
		{
			add_to_history(this_cmd);
			set_input(empty_string);
		}
	}
	else if (*com == '@')
	{
		/* This kludge fixes a memory leak */
		u_char	*tmp;

		tmp = parse_inline(line + 1, sub_args, &args_flag);
		if (tmp)
			new_free(&tmp);
		if (hist_flag && add_to_hist)
		{
			add_to_history(this_cmd);
			set_input(empty_string);
		}
	}
	else
	{
		u_char	*rest,
			*nalias = NULL,
			*alias_name;
		int	cmd_cnt,
			alias_cnt;
		IrcCommand	*command; /* = (IrcCommand *) 0 */

		display = window_display;
		old_display_var = (unsigned) get_int_var(DISPLAY_VAR);
		if ((rest = my_index(com, ' ')) != NULL)
			*(rest++) = (u_char) 0;
		else
			rest = empty_string;
		upper(com);

		/* first, check aliases */
		if (*com && my_index(cmdchars, *com))
		{
			alias_cnt = 0;
			com++;
			if (*com == '^')
			{
				com++;
				window_display = 0;
			}
		}
		else
		{
			if (*com == '^')
			{
				com++;
				window_display = 0;
			}
			nalias = get_alias(COMMAND_ALIAS, com, &alias_cnt,
				&alias_name);
		}
		if (nalias && (alias_cnt == 0))
		{
			if (hist_flag && add_to_hist)
			{
				add_to_history(this_cmd);
				set_input(empty_string);
			}
			execute_alias(alias_name, nalias, rest);
			new_free(&alias_name);
		}
		else
		{
			/* History */
			if (*com == '!')
			{
				if ((com = do_history(com + 1, rest)) != NULL)
				{
					if (level == 1)
					{
						set_input(com);
						update_input(UPDATE_ALL);
					}
					else
						parse_command(com, 0, sub_args);
					new_free(&com);
				}
				else
					set_input(empty_string);
			}
			else
			{
				if (hist_flag && add_to_hist)
				{
					add_to_history(this_cmd);
					set_input(empty_string);
				}
				command = find_command(com, &cmd_cnt);
				if ((command && cmd_cnt < 0) || (0 == alias_cnt && 1 == cmd_cnt))
				{
					if ((command->flags & SERVERREQ) && connected_to_server == 0)
						say("%s: You are not connected to a server. Use /SERVER to connect.", com);
					else if ((command->flags & NOICB) && get_server_version(from_server) == ServerICB)
						say("%s: Not available for ICB.", command->name);
					else if (command->func)
						command->func(UP(command->server_func), rest, sub_args);
					else
						say("%s: command disabled", command->name);
				}
				else if (nalias && 1 == alias_cnt && cmd_cnt == 1 && !my_strcmp(alias_name, command[0].name))
					execute_alias(alias_name, nalias, rest);
				else if ((alias_cnt + cmd_cnt) > 1)
					say("Ambiguous command: %s", com);
				else if (nalias && 1 == alias_cnt)
					execute_alias(alias_name, nalias, rest);
				else if (!my_stricmp(com, nickname))
						/* nick = /me  -lynx */
					me(NULL, rest, empty_string);
				else
					say("Unknown command: %s", com);
			}
			if (nalias)
				new_free(&alias_name);
		}
		if (old_display_var != get_int_var(DISPLAY_VAR))
			window_display = get_int_var(DISPLAY_VAR);
		else
			window_display = display;
	}
	new_free(&this_cmd);
	level--;
}
