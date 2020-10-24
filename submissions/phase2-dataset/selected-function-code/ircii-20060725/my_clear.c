my_clear(command, args, subargs)
	u_char	*command,
		*args,
		*subargs;
{
	u_char	*arg;
	int	all = 0,
		unhold = 0;

	while ((arg = next_arg(args, &args)) != NULL)
	{
		upper(arg);
		/* -ALL and ALL here becuase the help files used to be wrong */
		if (!my_strncmp(arg, "ALL", my_strlen(arg))
				|| !my_strncmp(arg, "-ALL", my_strlen(arg)))
			all = 1;
		else if (!my_strncmp(arg, "-UNHOLD", my_strlen(arg)))
			unhold = 1;
		else
			say("Unknown flag: %s", arg);
	}
	if (all)
		clear_all_windows(unhold);
	else
	{
		if (unhold)
			hold_mode((Window *) 0, OFF, 1);
		clear_window_by_refnum(0);
	}
	update_input(UPDATE_JUST_CURSOR);
}
