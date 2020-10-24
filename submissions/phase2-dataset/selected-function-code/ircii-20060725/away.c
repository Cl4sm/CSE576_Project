away(command, args, subargs)
	u_char	*command,
		*args,
		*subargs;
{
	size_t	len;
	u_char	*arg = NULL;
	int	flag = AWAY_ONE;
	int	i;

	if (*args)
	{
		if (*args == '-')
		{
			u_char	*cmd = (u_char *) 0;

			args = next_arg(args, &arg);
			len = my_strlen(args);
			if (len == 0)
			{
				say("%s: No argument given with -", command);
				return;
			}
			malloc_strcpy(&cmd, args);
			upper(cmd);
			if (0 == my_strncmp(cmd, "-ALL", len))
			{
				flag = AWAY_ALL;
				args = arg;
			}
			else if (0 == my_strncmp(cmd, "-ONE", len))
			{
				flag = AWAY_ONE;
				args = arg;
			}
			else
			{
				say("%s: %s unknown flag", command, args);
				new_free(&cmd);
				return;
			}
			new_free(&cmd);
		}
	}
	if (flag == AWAY_ALL)
		if (*args)
		{
			away_set = 1;
			MarkAllAway(command, args);
		}
		else
		{
			away_set = 0;
			for(i = 0; (i < number_of_servers); i++)
				if (server_list[i].whois_stuff.away)
					new_free(&(server_list[i].away));
		}
	else
	{
		send_to_server("%s :%s", command, args);
		if (*args)
		{
			away_set = 1;
			malloc_strcpy(&(server_list[
				curr_scr_win->server].away), args);
		}
		else
		{
			new_free(&(server_list[
				curr_scr_win->server].away));
			away_set = 0;
			for(i = 0; (i < number_of_servers) && !away_set ; i++)
				if (server_list[i].read != -1 &&
						server_list[i].away)
					away_set = 1;
		}
	}
	update_all_status();
}
