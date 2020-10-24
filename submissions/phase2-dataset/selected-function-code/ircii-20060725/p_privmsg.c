static	void
p_privmsg(from, Args)
	u_char	*from,
		**Args;
{
	int	level,
		flag,
		list_type,
		flood_type,
		log_type;
	u_char	ignore_type;
	u_char	*ptr,
		*to;
	u_char	*high;
	int	no_flood;

	if (!from)
		return;
	PasteArgs(Args, 1);
	to = Args[0];
	ptr = Args[1];
	if (!to || !ptr)
		return;
	save_message_from();
	if (is_channel(to))
	{
		malloc_strcpy(&public_nick, from);
		if (!is_on_channel(to, parsing_server_index, from))
		{
			log_type = LOG_PUBLIC;
			ignore_type = IGNORE_PUBLIC;
			list_type = PUBLIC_MSG_LIST;
			flood_type = PUBLIC_FLOOD;
		}
		else
		{
			log_type = LOG_PUBLIC;
			ignore_type = IGNORE_PUBLIC;
			if (is_current_channel(to, parsing_server_index, 0))
				list_type = PUBLIC_LIST;
			else
				list_type = PUBLIC_OTHER_LIST;
			flood_type = PUBLIC_FLOOD;
		}
		message_from(to, log_type);
	}
	else
	{
		flood_type = MSG_FLOOD;
		if (my_stricmp(to, get_server_nickname(parsing_server_index)))
		{
			log_type = LOG_WALL;
			ignore_type = IGNORE_WALLS;
			list_type = MSG_GROUP_LIST;
		}
		else
		{
			log_type = LOG_MSG;
			ignore_type = IGNORE_MSGS;
			list_type = MSG_LIST;
		}
		message_from(from, log_type);
	}
	flag = double_ignore(from, FromUserHost, ignore_type);
	switch (flag)
	{
	case IGNORED:
		if ((list_type == MSG_LIST) && get_int_var(SEND_IGNORE_MSG_VAR))
			send_to_server("NOTICE %s :%s is ignoring you", from,
					get_server_nickname(parsing_server_index));
		goto out;
	case HIGHLIGHTED:
		high = &highlight_char;
		break;
	default:
		high = empty_string;
		break;
	}
	level = set_lastlog_msg_level(log_type);
	ptr = do_ctcp(from, to, ptr);
	if (!ptr || !*ptr)
		goto out;
	if ((flag != DONT_IGNORE) && (ignore_usernames & ignore_type) && !FromUserHost)
		add_to_whois_queue(from, whois_ignore_msgs, "%s", ptr);
	else
	{
		no_flood = check_flooding(from, to, flood_type, ptr);
		if (ctcp_was_crypted == 0 || do_hook(ENCRYPTED_PRIVMSG_LIST,"%s %s %s",from, to, ptr))
		{
		switch (list_type)
		{
		case PUBLIC_MSG_LIST:
			if (no_flood && do_hook(list_type, "%s %s %s", from, to, ptr))
			    put_it("%s(%s/%s)%s %s", high, from, to, high, ptr);
			break;
		case MSG_GROUP_LIST:
			if (no_flood && do_hook(list_type, "%s %s %s", from, to, ptr))
			    put_it("%s-%s:%s-%s %s", high, from, to, high, ptr);
			break;
		case MSG_LIST:
			if (!no_flood)
				break;
			malloc_strcpy(&recv_nick, from);
			if (away_set)
				beep_em(get_int_var(BEEP_WHEN_AWAY_VAR));
			if (do_hook(list_type, "%s %s", from, ptr))
			{
			    if (away_set)
			    {
				time_t t;
				u_char *msg = (u_char *) 0;
				size_t len = my_strlen(ptr) + 20;

				t = time((time_t *) 0);
				msg = (u_char *) new_malloc(len);
				snprintf(CP(msg), len, "%s <%.16s>", ptr, ctime(&t));
				put_it("%s*%s*%s %s", high, from, high, msg);
				new_free(&msg);
			    }
			    else
				put_it("%s*%s*%s %s", high, from, high, ptr);
			}
			break;
		case PUBLIC_LIST:
			if (get_int_var(MAKE_NOTICE_MSG_VAR))
				doing_privmsg = 1;
			if (no_flood && do_hook(list_type, "%s %s %s", from, 
			    to, ptr))
				put_it("%s<%s>%s %s", high, from, high, ptr);
			doing_privmsg = 0;
			break;
		case PUBLIC_OTHER_LIST:
			if (get_int_var(MAKE_NOTICE_MSG_VAR))
				doing_privmsg = 1;
			if (no_flood && do_hook(list_type, "%s %s %s", from,
			    to, ptr))
				put_it("%s<%s:%s>%s %s", high, from, to, high,
					ptr);
			doing_privmsg = 0;
			break;
		}
		if (beep_on_level & log_type)
			beep_em(1);
		}
	}
	set_lastlog_msg_level(level);
out:
	restore_message_from();
}
