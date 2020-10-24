parse_server_notice(from, to, line)
	u_char	*from,
		*to,
		*line;
{
	u_char	server[81],
		version[21];
	int	user_cnt,
		server_cnt,
		lastlog_level;
	int	flag;

	if (!from || !*from)
		from = server_list[parsing_server_index].itsname ?
			server_list[parsing_server_index].itsname :
			server_list[parsing_server_index].name;
	if (get_int_var(SUPPRESS_SERVER_MOTD_VAR) &&
		get_server_motd(parsing_server_index))
	{
		if (my_strncmp("*** Message-of-today", line, 20) == 0)
		{
			set_server_motd(parsing_server_index, 0);
			return;
		}
		if (my_strncmp("MOTD ", line, 5) == 0)
		{
			set_server_motd(parsing_server_index, 1);
			return;
		}
		if (my_strcmp("* End of /MOTD command.", line) == 0)
		{
			set_server_motd(parsing_server_index, 0);
			return;
		}
	}
	save_message_from();
	if (!my_strncmp(line, "*** Notice --", 13))
	{
		message_from((u_char *) 0, LOG_OPNOTE);
		lastlog_level = set_lastlog_msg_level(LOG_OPNOTE);
	}
	else
	{
		message_from((u_char *) 0, LOG_SNOTE);
		lastlog_level = set_lastlog_msg_level(LOG_SNOTE);
	}
	if (to)
	{
		if (do_hook(SERVER_NOTICE_LIST, "%s %s %s", from, to, line))
			put_it("%s %s", to, line);
	}
	else
	{
		if (get_server_version(parsing_server_index) >= Server2_7 && 
		    *line != '*'  && *line != '#' && my_strncmp(line, "MOTD ", 4))
			flag = 1;
		else
			flag = 0;
		if (do_hook(SERVER_NOTICE_LIST, flag ? "%s *** %s"
						     : "%s %s", from, line))
			put_it(flag ? "*** %s" : "%s", line);
	}
	if ((parsing_server_index == primary_server) &&
			((sscanf(CP(line), "*** There are %d users on %d servers",
			&user_cnt, &server_cnt) == 2) ||
			(sscanf(CP(line), "There are %d users on %d servers",
			&user_cnt, &server_cnt) == 2)))
	{
		if ((server_cnt < get_int_var(MINIMUM_SERVERS_VAR)) ||
				(user_cnt < get_int_var(MINIMUM_USERS_VAR)))
		{
			say("Trying better populated server...");
			get_connected(parsing_server_index + 1);
		}
	}
#ifdef BROKEN_SCANF
	else if (!my_strncmp(line, "*** Your host is ", 17))
#else
	else if ((sscanf(CP(line), "*** Your host is %80s running version %20s",
			server, version) == 2))
#endif /* BROKEN_SCANF */
	{
		if (get_server_version(parsing_server_index) < Server2_8)
			got_initial_version(line);
	}
	if (lastlog_level)
	{
		set_lastlog_msg_level(lastlog_level);
		restore_message_from();
	}
}
