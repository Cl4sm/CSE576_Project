void 
parse_notice(from, Args)
	u_char 	*from;
	u_char 	**Args;
{
	int	level,
		type;
	u_char	*to;
	int	no_flooding;
	int	flag;
	u_char	*high,
		not_from_server = 1;
	u_char	*line;

	PasteArgs(Args, 1);
	to = Args[0];
	line = Args[1];
	if (!to || !line)
		return;
	if (*to)
	{
		save_message_from();
		if (is_channel(to))
		{
			message_from(to, LOG_NOTICE);
			type = PUBLIC_NOTICE_LIST;
		}
		else
		{
			message_from(from, LOG_NOTICE);
			type = NOTICE_LIST;
		}
		if (from && *from && my_strcmp(get_server_itsname(parsing_server_index), from))
		{
			flag = double_ignore(from, FromUserHost,IGNORE_NOTICES);
			if (flag != IGNORED)
			{
				if (flag == HIGHLIGHTED)
					high = &highlight_char;
				else
					high = empty_string;
				if (my_index(from, '.'))
				{
		/*
		 * only dots in servernames, right ?
		 *
		 * But a server name doesn't nessicarily have to have
		 * a 'dot' in it..  - phone, jan 1993.
		 */
					not_from_server = 0;
					if (my_strncmp(line, "*/", 2) == 0)
					{
						parse_note(from, line + 1);
						goto out;
					}
				}
				if (not_from_server && (flag != DONT_IGNORE) && !FromUserHost &&
							(ignore_usernames & IGNORE_NOTICES))
					add_to_whois_queue(from, whois_ignore_notices, "%s %s", to, line);
				else
				{
					line = do_notice_ctcp(from, to, line);
					if (*line == '\0')
					{
						goto out;
					}
					level = set_lastlog_msg_level(LOG_NOTICE);
					no_flooding = check_flooding(from, to, NOTICE_FLOOD, line);

					if (no_flooding &&
					   (ctcp_was_crypted == 0 || do_hook(ENCRYPTED_NOTICE_LIST, "%s %s %s", from, to, line)))
					{
						if (do_hook(type, "%s %s %s", from, to, line))
						{
							if (type == NOTICE_LIST)
								put_it("%s-%s-%s %s", high, from, high, line);
							else
								put_it("%s-%s:%s-%s %s", high, from, to, high, line);
						}
						if (beep_on_level & LOG_NOTICE)
							beep_em(1);
						set_lastlog_msg_level(level);
						if (not_from_server)
							notify_mark(from, 1, 0);
					}
				}
			}
		}
		else 
			parse_server_notice(from, type == PUBLIC_NOTICE_LIST ? to : 0, line);
	}
	else
		put_it("%s", line + 1);
out:
	restore_message_from();
}
