send_text(org_nick, line, command)
	u_char	*org_nick;
	u_char	*line;
	u_char	*command;
{
	crypt_key	*key;
	u_char 	*ptr,
		*free_nick,
		*nick = NULL;
	int	lastlog_level,
		list_type,
		old_server;
	int	check_away = 0;
	u_char	the_thing;
	u_char	*query_command = NULL;
	u_char	nick_list[IRCD_BUFFER_SIZE];
	int	do_final_send = 0;
	int	is_icb = get_server_version(from_server) == ServerICB;
	
	*nick_list = '\0';
	malloc_strcpy(&nick, org_nick);
	free_nick = ptr = nick;
	save_message_from();
	while ((nick = ptr) != NULL)
	{
		if (is_icb && is_current_channel(nick, curr_scr_win->server, 0))
			ptr = NULL;
		else if ((ptr = my_index(nick, ',')) != NULL)
			*(ptr++) = (u_char) 0;
		if (!*nick)
			continue;
#ifndef _Windows
		if (is_process(nick))
		{
			int	i;

			if ((i = get_process_index(&nick)) == -1)
				say("Invalid process specification");
			else
				text_to_process(i, line, 1);
			continue;
		}
#endif /* _Windows */
		if (!*line)
			continue; /* lynx */
		if (in_on_who && *nick != '=') /* allow dcc messages anyway */
		{
			say("You may not send messages from ON WHO, ON WHOIS, or ON JOIN");
			continue;
		}
		if (doing_privmsg)
			command	= UP("NOTICE");
		if (is_current_channel(nick, curr_scr_win->server, 0))
		{
			/* nothing */
		}
		else if (*nick == '\\') /* quoteme */
			nick++;
		else if (my_strcmp(nick, "\"") == 0) /* quote */
		{
			send_to_server("%s", line);
			continue;
		}
		else if (*nick == '=') /* DCC chat */
		{
			old_server = from_server;
			from_server = -1;
			dcc_chat_transmit(nick + 1, line);
			from_server = old_server;
			continue;
		}
		else if (*nick == '/') /* Command */
		{
			malloc_strcpy(&query_command, nick);
			malloc_strcat(&query_command, UP(" "));
			malloc_strcat(&query_command, line);
			parse_command(query_command, 0, empty_string);
			new_free(&query_command);
			continue;
		}
		if (send_text_flag == NOTICE_LIST)
		{
			say("You cannot send a message from a NOTICE");
			new_free(&free_nick);
			goto out;
		}
		if (is_icb)
		{
			if (my_stricmp(nick, get_server_icbgroup(from_server)) != 0 ||
			    (command && my_strcmp(command, "PRIVMSG") == 0)) {
				icb_put_msg2(nick, line);
				malloc_strcpy(&sent_nick, nick);
			} else
				icb_put_public(line);
		}
		else
		{	/* special */
		if (send_text_flag == MSG_LIST)
			command = UP("NOTICE");
		if (is_channel(nick))
		{
			int	current;

			current = is_current_channel(nick, curr_scr_win->server, 0);
			if (!command || my_strcmp(command, "NOTICE"))
			{
				check_away = 1;
				command = UP("PRIVMSG");
				lastlog_level = set_lastlog_msg_level(LOG_PUBLIC);
				message_from(nick, LOG_PUBLIC);
				list_type = SEND_PUBLIC_LIST;
				the_thing = '>';
			}
			else
			{
				lastlog_level = set_lastlog_msg_level(LOG_NOTICE);
				message_from(nick, LOG_NOTICE);
				list_type = SEND_NOTICE_LIST;
				the_thing = '-';
			}
			if (do_hook(list_type, "%s %s", nick, line))
			{
				if (current)
					put_it("%c %s", the_thing, line);
				else
					put_it("%c%s> %s", the_thing, nick,
						line);
			}
			set_lastlog_msg_level(lastlog_level);
			if ((key = is_crypted(nick)) != 0)
			{
				u_char	*crypt_line;

				if ((crypt_line = crypt_msg(line, key, 1)))
					send_to_server("%s %s :%s", command, nick, crypt_line);
				continue;
			}
			if (!in_on_who)
			{
				if (*nick_list)
				{
					my_strmcat(nick_list, ",", sizeof nick_list);
					my_strmcat(nick_list, nick, sizeof nick_list);
				}
				else
					my_strmcpy(nick_list, nick, sizeof nick_list);
			}
			do_final_send = 1;
		}
		else
		{
			if (!command || my_strcmp(command, "NOTICE"))
			{
				check_away = 1;
				lastlog_level = set_lastlog_msg_level(LOG_MSG);
				command = UP("PRIVMSG");
				message_from(nick, LOG_MSG);
				list_type = SEND_MSG_LIST;
				the_thing = '*';
			}
			else
			{
				lastlog_level = set_lastlog_msg_level(LOG_NOTICE);
				message_from(nick, LOG_NOTICE);
				list_type = SEND_NOTICE_LIST;
				the_thing = '-';
			}
			if (window_display && do_hook(list_type, "%s %s", nick, line))
				put_it("-> %c%s%c %s", the_thing, nick, the_thing, line);
			if ((key = is_crypted(nick)) != NULL)
			{
				u_char	*crypt_line;

				if ((crypt_line = crypt_msg(line, key, 1)))
					send_to_server("%s %s :%s", command ? command : (u_char *) "PRIVMSG", nick, crypt_line);
				continue;
			}
			set_lastlog_msg_level(lastlog_level);

			if (!in_on_who)
			{
				if (*nick_list)
				{
					my_strmcat(nick_list, ",", sizeof nick_list);
					my_strmcat(nick_list, nick, sizeof nick_list);
				}
				else
					my_strmcpy(nick_list, nick, sizeof nick_list);
			}

			if (get_int_var(WARN_OF_IGNORES_VAR) && (is_ignored(nick, IGNORE_MSGS) == IGNORED))
				say("Warning: You are ignoring private messages from %s", nick);

			malloc_strcpy(&sent_nick, nick);
			do_final_send = 1;
		}
		}	/* special */
	}
	if (check_away && server_list[curr_scr_win->server].away && get_int_var(AUTO_UNMARK_AWAY_VAR) &&
	    get_server_version(from_server) != ServerICB)
		away(UP("AWAY"), empty_string, empty_string);

	malloc_strcpy(&sent_body, line);
	if (do_final_send)
		send_to_server("%s %s :%s", command ? command : (u_char *) "PRIVMSG", nick_list, line);
	new_free(&free_nick);
out:
	restore_message_from();
}
