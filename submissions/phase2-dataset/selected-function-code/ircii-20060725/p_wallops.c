p_wallops(from, ArgList)
	u_char	*from,
		**ArgList;
{
	int	flag, level;
	u_char	*line;

	if (!from)
		return;
	if (!(line = PasteArgs(ArgList, 0)))
		return;
	flag = double_ignore(from, FromUserHost, IGNORE_WALLOPS);
	level = set_lastlog_msg_level(LOG_WALLOP);
	save_message_from();
	message_from(from, LOG_WALLOP);
	if (my_index(from, '.'))
	{
		u_char	*high;

		if (flag != IGNORED)
		{
			if (flag == HIGHLIGHTED)
				high = &highlight_char;
			else
				high = empty_string;
			if (do_hook(WALLOP_LIST, "%s S %s", from, line))
				put_it("%s!%s!%s %s", high, from, high, line);
			if (beep_on_level & LOG_WALLOP)
				beep_em(1);
		}
	}
	else
	{
		if (get_int_var(USER_WALLOPS_VAR))
		{
			if ((flag != DONT_IGNORE) && (check_flooding(from, get_server_nickname(parsing_server_index), WALLOP_FLOOD, line)))
			add_to_whois_queue(from, whois_new_wallops, "%s", line);
		}
		else if (my_strcmp(from, get_server_nickname(get_window_server(0))) != 0)
			put_it("!%s! %s", from, line);
	}
	set_lastlog_msg_level(level);
	restore_message_from();
}
