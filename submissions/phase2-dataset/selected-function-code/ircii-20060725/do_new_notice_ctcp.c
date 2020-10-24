static	void
do_new_notice_ctcp(from, to, str, cmd)
	u_char	*from,
		*to,
		**str,
		*cmd;
{
	u_char	*end,
		*args,
		*ptr,
		*arg_copy = NULL;
	int	flags,
		i,
		lastlog_level;

	flags = 0;
	*(cmd++) = '\0';
	my_strmcat(ctcp_buffer, *str, sizeof ctcp_buffer);
	if ((end = my_index(cmd, CTCP_DELIM_CHAR)) != NULL)
	{
		*(end++) = '\0';
		if ((args = my_index(cmd, ' ')) != NULL)
			*(args++) = '\0';
		malloc_strcpy(&arg_copy, args);
		for (i = 0; i < NUMBER_OF_CTCPS; i++)
		{
			if ((my_strcmp(cmd, ctcp_cmd[i].name) == 0) && ctcp_cmd[i].flag & CTCP_NOREPLY)
			{
				if ((ptr = ctcp_cmd[i].func(&(ctcp_cmd[i]), from, to, arg_copy)) != NULL)
				{
					my_strmcat(ctcp_buffer, ptr, sizeof ctcp_buffer);
					new_free(&ptr);
					flags = ctcp_cmd[i].flag;
				}
				break;
			}
		}
		new_free(&arg_copy);
		if (!args)
			args = empty_string;
		if (do_hook(CTCP_REPLY_LIST, "%s %s %s %s", from, to, cmd,
				args) && !(flags & CTCP_NOREPLY))
		{
			if (!my_strcmp(cmd, "PING"))
			{
				u_char	buf[20];
				time_t	timediff,
					currenttime;

				currenttime = time(NULL);
				if (args && *args)
					timediff = currenttime -
						(time_t) my_atol(args);
				else
					timediff = (time_t) 0;
				snprintf(CP(buf), sizeof buf, "%ld second%s",
				   (long) timediff, (timediff == 1) ? "" : "s");
				args = buf;
			}
			save_message_from();
			lastlog_level = set_lastlog_msg_level(LOG_CTCP);
			message_from((u_char *) 0, LOG_CTCP);
			say("CTCP %s reply from %s: %s", cmd, from,
				args);
			set_lastlog_msg_level(lastlog_level);
			restore_message_from();
		}
		*str = end;
	}
	else
	{
		my_strmcat(ctcp_buffer, CTCP_DELIM_STR, sizeof ctcp_buffer);
		*str = cmd;
	}
}
