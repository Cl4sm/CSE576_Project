set_variable(command, args, subargs)
	u_char	*command,
		*args,
		*subargs;
{
	u_char	*var;
	int	cnt,
		var_index,
		lastlog_level;

	if ((var = next_arg(args, &args)) != NULL)
	{
		if (*var == '-')
		{
			var++;
			args = (u_char *) 0;
		}
		var_index = find_variable(var, &cnt);
		switch (cnt)
		{
		case 0:
			say("No such variable \"%s\"", var);
			return;
		case 1:
			set_var_value(var_index, args);
			return;
		default:
			say("%s is ambiguous", var);
			for (cnt += var_index; var_index < cnt; var_index++)
				set_var_value(var_index, empty_string);
			return;
		}
	}
	lastlog_level = message_from_level(LOG_CRAP);
	for (var_index = 0; var_index < NUMBER_OF_VARIABLES; var_index++)
		set_var_value(var_index, empty_string);
	(void) message_from_level(lastlog_level);
}
