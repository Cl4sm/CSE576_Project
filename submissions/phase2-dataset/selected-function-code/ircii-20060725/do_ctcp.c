do_ctcp(from, to, str)
	u_char	*from,
		*to,
		*str;
{
	int	i = 0,
		ctcp_flag = 1;
	u_char	*end,
		*cmd,
		*args,
		*ptr;
	u_char	*arg_copy = NULL;
	int	flag;
	int	messages = 0;
	time_t	curtime = time(NULL);

	flag = double_ignore(from, FromUserHost, IGNORE_CTCPS);

	if (!in_ctcp_flag)
		in_ctcp_flag = 1;
	*ctcp_buffer = '\0';
	while ((cmd = my_index(str, CTCP_DELIM_CHAR)) != NULL)
	{
		if (messages > 3)
			break;
		*(cmd++) = '\0';
		my_strmcat(ctcp_buffer, str, sizeof ctcp_buffer);
		if ((end = my_index(cmd, CTCP_DELIM_CHAR)) != NULL)
		{
			messages++;
			if (flag == IGNORED)
				continue;
			*(end++) = '\0';
			if ((args = my_index(cmd, ' ')) != NULL)
				*(args++) = '\0';
			else
				args = empty_string;
#if 0
			/* Skip leading : for arguments */
			if (*args == ':')
				++args;
#endif
			malloc_strcpy(&arg_copy, args);
			for (i = 0; i < NUMBER_OF_CTCPS; i++)
			{
				if (my_strcmp(cmd, ctcp_cmd[i].name) == 0)
				{
					/* protect against global (oper) messages */
					if (*to != '$' && !(*to == '#' && !lookup_channel(to, parsing_server_index, CHAN_NOUNLINK)))
					{
						ptr = ctcp_cmd[i].func(&ctcp_cmd[i], from, to, arg_copy);
						if (ptr)
						{
							my_strmcat(ctcp_buffer, ptr, sizeof ctcp_buffer);
							new_free(&ptr);
						}
					}
					ctcp_flag = ctcp_cmd[i].flag;
					cmd = ctcp_cmd[i].name;
					break;
				}
			}
			new_free(&arg_copy);
			if (in_ctcp_flag == 1 &&
			    do_hook(CTCP_LIST, "%s %s %s %s", from, to, cmd,
			    args) && get_int_var(VERBOSE_CTCP_VAR))
			{
				int     lastlog_level;

				save_message_from();
				lastlog_level = set_lastlog_msg_level(LOG_CTCP);
				message_from((u_char *) 0, LOG_CTCP);
				if (i == NUMBER_OF_CTCPS)
				{
					if (beep_on_level & LOG_CTCP)
						beep_em(1);
					say("Unknown CTCP %s from %s to %s: %s%s",
						cmd, from, to, *args ?
						(u_char *) ": " : empty_string, args);
				}
				else if (ctcp_flag & CTCP_VERBOSE)
				{
					if (beep_on_level & LOG_CTCP)
						beep_em(1);
					if (my_stricmp(to,
					    get_server_nickname(parsing_server_index)))
						say("CTCP %s from %s to %s: %s",
							cmd, from, to, args);
					else
						say("CTCP %s from %s%s%s", cmd,
							from, *args ? (u_char *)
							": " : empty_string, args);
				}
				set_lastlog_msg_level(lastlog_level);
				restore_message_from();
			}
			str = end;
		}
		else
		{
			my_strmcat(ctcp_buffer, CTCP_DELIM_STR, sizeof ctcp_buffer);
			str = cmd;
		}
	}
	if (in_ctcp_flag == 1)
		in_ctcp_flag = 0;
	if (CTCP_Reply_Buffer && *CTCP_Reply_Buffer)
	{
#ifdef PARANOID
		/*
		 * paranoid users don't want to send ctcp replies to
		 * requests send to channels, probably...
		 */
		if (is_channel(to))
			goto clear_ctcp_reply_buffer;
#endif
		/*
		 * Newave ctcp flood protection : each time you are requested to send
		 * more than CTCP_REPLY_FLOOD_SIZE bytes in CTCP_REPLY_BACKLOG_SECONDS
		 * no ctcp replies will be done for CTCP_REPLY_IGNORE_SECONDS.
		 * Current default is 256 bytes/ 5s/ 10s
		 * This is a sliding window, i.e. you can't get caught sending too much
		 * because of a 5s boundary, and the checking is still active even if
		 * you don't reply anymore.
		 */

		if (*from == '=')
			send_ctcp(ctcp_type[CTCP_NOTICE], from, NULL, "%s", CTCP_Reply_Buffer);
		else
		{
			Server	*cur_serv = &server_list[parsing_server_index];
			int	no_reply,
				no_flood = get_int_var(NO_CTCP_FLOOD_VAR),
				delta = cur_serv->ctcp_last_reply_time ? curtime-cur_serv->ctcp_last_reply_time : 0,
				size = 0,
				was_ignoring = cur_serv->ctcp_flood_time != 0,
				crbs = get_int_var(CTCP_REPLY_BACKLOG_SECONDS_VAR),
				crfs = get_int_var(CTCP_REPLY_FLOOD_SIZE_VAR),
				cris = get_int_var(CTCP_REPLY_IGNORE_SECONDS_VAR);

			cur_serv->ctcp_last_reply_time = curtime;

			if (delta)
			{
				for (i = crbs - 1; i >= delta; i--)
					cur_serv->ctcp_send_size[i] = cur_serv->ctcp_send_size[i - delta];
				for (i = 0; i < delta && i < crbs; i++)
					cur_serv->ctcp_send_size[i] = 0;
			}

			cur_serv->ctcp_send_size[0] += my_strlen(CTCP_Reply_Buffer);

			for (i = 0; i < crbs; i++)
				size += cur_serv->ctcp_send_size[i];
			if (size >= crfs)
				cur_serv->ctcp_flood_time = curtime;

			no_reply = cur_serv->ctcp_flood_time && (curtime <= cur_serv->ctcp_flood_time+cris);

			if (no_flood && get_int_var(VERBOSE_CTCP_VAR))
			{
				save_message_from();
				message_from((u_char *) 0, LOG_CTCP);
				if (no_reply && was_ignoring == 0)
					say("CTCP flood detected - suspending replies");
				else if (no_reply == 0 && was_ignoring)
					say("CTCP reply suspending time elapsed - replying normally");
				restore_message_from();
			}
			if (no_flood == 0 || no_reply == 0)
			{
				cur_serv->ctcp_flood_time = 0;
				send_ctcp(ctcp_type[CTCP_NOTICE], from, NULL, "%s", CTCP_Reply_Buffer);
			}
		}
#ifdef PARANOID
clear_ctcp_reply_buffer:
#endif
		*CTCP_Reply_Buffer = '\0';
	}
	if (*str)
		my_strmcat(ctcp_buffer, str, sizeof ctcp_buffer);
	return (ctcp_buffer);
}
