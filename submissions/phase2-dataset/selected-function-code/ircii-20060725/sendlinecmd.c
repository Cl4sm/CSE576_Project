static	void
sendlinecmd(command, args, subargs)
	u_char	*command,
		*args,
		*subargs;
{
	int	server;
	int	display;

	server = from_server;
	display = window_display;
	window_display = 1;
	if (get_int_var(INPUT_ALIASES_VAR))
		parse_line(NULL, args, empty_string, 1, 0, 0);
	else
		parse_line(NULL, args, NULL, 1, 0, 0);
	update_input(UPDATE_ALL);
	window_display = display;
	from_server = server;
}
