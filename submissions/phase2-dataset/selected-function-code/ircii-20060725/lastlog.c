lastlog(command, args, subargs)
	u_char	*command,
		*args,
		*subargs;
{
	int	cnt,
		from = 0,
		p,
		i,
		level = 0,
		m_level,
		mask = 0,
		header = 1;
	Lastlog *start_pos;
	u_char	*match = NULL,
		*save = NULL,
		*expanded = NULL,
		*arg;
	FILE	*fp = NULL;
	u_char	*cmd = (u_char *) 0;
	size_t	len;

	save_message_from();
	message_from((u_char *) 0, LOG_CURRENT);
	cnt = curr_scr_win->lastlog_size;

	while ((arg = next_arg(args, &args)) != NULL)
	{
		if (*arg == '-')
		{
			arg++;
			if (!(len = my_strlen(arg)))
			{
				header = 0;
				continue;
			}
			malloc_strcpy(&cmd, arg);
			upper(cmd);
			if (!my_strncmp(cmd, "LITERAL", len))
			{
				if (match)
				{
					say("Second -LITERAL argument ignored");
					(void) next_arg(args, &args);
					continue;
				}
				if ((match = next_arg(args, &args)) != NULL)
					continue;
				say("Need pattern for -LITERAL");
				goto out;
			}
			else if (!my_strncmp(cmd, "BEEP", len))
			{
				if (match)
				{
					say("-BEEP is exclusive; ignored");
					continue;
				}
				else
					match = UP("\007");
			}
			else if (!my_strncmp(cmd, "SAVE", len))
			{
#ifdef DAEMON_UID
				if (getuid() == DAEMON_UID)
				{
					say("You are not permitted to use -SAVE flag");
					goto out;
				}
#endif /* DAEMON_UID */
				if (save)
				{
					say("Second -SAVE argument ignored");
					(void) next_arg(args, &args);
					continue;
				}
				if ((save = next_arg(args, &args)) != NULL)
				{
					if (!(expanded = expand_twiddle(save)))
					{
						say("Unknown user");
						goto out;
					}
					if ((fp = fopen(CP(expanded), "w")) != NULL)
						continue;
					say("Error opening %s: %s", save, strerror(errno));
					goto out;
				}
				say("Need filename for -SAVE");
				goto out;
			}
			else
			{
				for (i = 0, p = 1; i < NUMBER_OF_LEVELS; i++, p <<= 1)
				{
					if (my_strncmp(cmd, levels[i], len) == 0)
					{
						mask |= p;
						break;
					}
				}
				if (i == NUMBER_OF_LEVELS)
				{
					say("Unknown flag: %s", arg);
					message_from((u_char *) 0, LOG_CRAP);
					goto out;
				}
			}
			continue;
out:
			new_free(&cmd);
			restore_message_from();
			return;
		}
		else
		{
			if (level == 0)
			{
				if (match || isdigit(*arg))
				{
					cnt = my_atoi(arg);
					level++;
				}
				else
					match = arg;
			}
			else if (level == 1)
			{
				from = my_atoi(arg);
				level++;
			}
		}
	}
	if (cmd)
		new_free(&cmd);

	start_pos = curr_scr_win->lastlog_head;
	for (i = 0; (i < from) && start_pos; start_pos = start_pos->next)
		if (!mask || (mask & start_pos->level))
			i++;

	for (i = 0; (i < cnt) && start_pos; start_pos = start_pos->next)
		if (!mask || (mask & start_pos->level))
			i++;

	level = curr_scr_win->lastlog_level;
	m_level = set_lastlog_msg_level(0);
	if (start_pos == (Lastlog *) 0)
		start_pos = curr_scr_win->lastlog_tail;
	else
		start_pos = start_pos->prev;

	/* Let's not get confused here, display a seperator.. -lynx */
	if (header && !save)
		say("Lastlog:");
	for (i = 0; (i < cnt) && start_pos; start_pos = start_pos->prev)
	{
		if (!mask || (mask & start_pos->level))
		{
			i++;
			if (match)
			{
				if (scanstr(start_pos->msg, match)) {
					if (save)
						fprintf(fp, "%s\n", start_pos->msg);
					else
						put_it("%s", start_pos->msg);
				}
			}
			else
				if (save)
					fprintf(fp, "%s\n", start_pos->msg);
				else
					put_it("%s", start_pos->msg);
		}
	}
	if (save)
	{
		say("Saved Lastlog to %s", expanded);
		fclose(fp);
	}
	else if (header)
		say("End of Lastlog");
	set_lastlog_msg_level(m_level);
	restore_message_from();
}
