p_invite(from, ArgList)
	u_char	*from,
		**ArgList;
{
	u_char	*high;
	int	flag;

	if (!from)
		return;
	flag = double_ignore(from, FromUserHost, IGNORE_INVITES);
	switch (flag)
	{
	case IGNORED:
		if (get_int_var(SEND_IGNORE_MSG_VAR))
			send_to_server("NOTICE %s :%s is ignoring you",
				from, get_server_nickname(parsing_server_index));
		return;
	case HIGHLIGHTED:
		high = &highlight_char;
		break;
	default:
		high = empty_string;
		break;
	}
	if (ArgList[0] && ArgList[1])
	{
		if ((flag != DONT_IGNORE) && (ignore_usernames & IGNORE_INVITES)
		    && !FromUserHost)
			add_to_whois_queue(from, whois_ignore_invites,
					"%s", ArgList[1]);
		else
		{
			save_message_from();
			message_from(from, LOG_CRAP);
			if (do_hook(INVITE_LIST, "%s %s", from, ArgList[1]))
				say("%s%s%s invites you to channel %s", high,
						from, high, ArgList[1]);
			restore_message_from();
			malloc_strcpy(&invite_channel, ArgList[1]);
			malloc_strcpy(&recv_nick, from);
		}
	}
}
