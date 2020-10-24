static	void
p_wall(from, ArgList)
	u_char	*from,
		**ArgList;
{
	int	flag,
		level;
	u_char	*line;
	u_char	*high;

	if (!from)
		return;
	PasteArgs(ArgList, 0);
	if (!(line = ArgList[0]))
		return;
	flag = double_ignore(from, FromUserHost, IGNORE_WALLS);
	save_message_from();
	message_from(from, LOG_WALL);
	if (flag != IGNORED)
	{
		if (flag == HIGHLIGHTED)
			high = &highlight_char;
		else
			high = empty_string;
		if ((flag != DONT_IGNORE) && (ignore_usernames & IGNORE_WALLS)
				&& !FromUserHost)
			add_to_whois_queue(from, whois_ignore_walls, "%s",line);
		else
		{
			level = set_lastlog_msg_level(LOG_WALL);
			if (check_flooding(from,
			    get_server_nickname(parsing_server_index),
			    WALL_FLOOD, line) && do_hook(WALL_LIST, "%s %s",
						     from, line))
				put_it("%s#%s#%s %s", high, from, high, line);
			if (beep_on_level & LOG_WALL)
				beep_em(1);
			set_lastlog_msg_level(level);
		}
	}
	restore_message_from();
}
