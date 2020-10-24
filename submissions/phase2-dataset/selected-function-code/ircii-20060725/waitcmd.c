static void
waitcmd(command, args, subargs)
	u_char	*command,
		*args,
		*subargs;
{
#ifdef _Windows
	yell("WAIT is not available under Windows");
#else /* Windows */
	int	wait_index;
	u_char	*flag;
	u_char	*procindex;
	int	cmd = 0;
	size_t	len;
	u_char	buffer[BIG_BUFFER_SIZE];

	while (args && *args == '-')
	{
		flag = next_arg(args, &args);
		len = my_strlen(++flag);
		if (!my_strnicmp(UP("CMD"), flag, len))
		{
			cmd = 1;
			break;
		}
		else
			yell("Unknown argument to WAIT: %s", flag);
	}
	if ((procindex = next_arg(args, &args)) && *procindex == '%' &&
			(wait_index = get_process_index(&procindex)) != -1)
	{
		if (is_process_running(wait_index))
		{
			if (cmd)
			{
				add_process_wait(wait_index, args?args:empty_string);
				return;
			}
			else
				set_wait_process(wait_index);
		}
		else
		{
			say("Not a valid process!");
			return;
		}
	}
	else if (cmd)
	{
		WaitCmd	*new;

		snprintf(CP(buffer), sizeof buffer, "%s %s", procindex, args);
		new = (WaitCmd *) new_malloc(sizeof(WaitCmd));
		new->stuff = NULL;
		malloc_strcpy(&new->stuff, buffer);
		new->next = NULL;
		if (end_wait_list)
			end_wait_list->next = new;
		end_wait_list = new;
		if (!start_wait_list)
			start_wait_list = new;
		send_to_server("%s", wait_nick);
		return;
	}
	else
		send_to_server("%s", lame_wait_nick);
	if (waiting)
		yell("WAIT has been called recursively.");

	waiting++;
	irc_io(NULL, NULL, 0, 1);
	waiting--;
#endif /* _Windows */
}
