get_from_history_buffer(which)
	int	which;
{
	if ((get_int_var(HISTORY_VAR) == 0) || (hist_size == 0))
		return (u_char *) 0;
	/*
	 * if (last_dir != which) { last_dir = which; get_from_history(which); }
	 */
	if (which == NEXT)
	{
		if (command_history_pos)
		{
			if (command_history_pos->prev)
				command_history_pos = command_history_pos->prev;
			else
				command_history_pos = command_history_tail;
		}
		else
		{
			add_to_history(get_input());
			command_history_pos = command_history_tail;
		}
		return (command_history_pos->stuff);
	}
	else
	{
		if (command_history_pos)
		{
			if (command_history_pos->next)
				command_history_pos = command_history_pos->next;
			else
				command_history_pos = command_history_head;
		}
		else
		{
			add_to_history(get_input());
			command_history_pos = command_history_head;
		}
		return (command_history_pos->stuff);
	}
}
