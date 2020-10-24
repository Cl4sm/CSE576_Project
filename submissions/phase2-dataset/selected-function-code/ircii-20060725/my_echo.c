my_echo(command, args, subargs)
	u_char	*command,
		*args,
		*subargs;
{
	unsigned int	display;
	int	lastlog_level = 0;
	int	from_level = 0;
	u_char	*flag_arg;
	int	temp;
	Window *old_to_window;

	save_message_from();
	old_to_window = to_window;
	if (command && *command == 'X')
	{
		while (args && *args == '-')
		{
			flag_arg = next_arg(args, &args);
			switch(flag_arg[1])
			{
			case 'L':
			case 'l':
				if (!(flag_arg = next_arg(args, &args)))
					break;
				if ((temp = parse_lastlog_level(flag_arg)) != 0)
				{
					lastlog_level = set_lastlog_msg_level(temp);
					from_level = message_from_level(temp);
					my_echo_set_message_from = 1;
				}
				break;
			case 'W':
			case 'w':
				if (!(flag_arg = next_arg(args, &args)))
					break;
				if (isdigit(*flag_arg))
					to_window = get_window_by_refnum((unsigned)my_atoi(flag_arg));
				else
					to_window = get_window_by_name(flag_arg);
				lastlog_level = set_lastlog_msg_level(LOG_CRAP);
				from_level = message_from_level(LOG_CRAP);
				break;
			}
			if (!args)
				args = empty_string;
		}
	}
	display = window_display;
	window_display = 1;
	put_it("%s", args);
	window_display = display;
	if (lastlog_level)
		set_lastlog_msg_level(lastlog_level);
	if (from_level)
		message_from_level(from_level);
	restore_message_from();
	to_window = old_to_window;
	my_echo_set_message_from = 0;
}
