send_line(key, ptr)
	u_int	key;
	u_char	*ptr;
{
	int	server;
	WaitPrompt	*OldPrompt;

	server = from_server;
	from_server = get_window_server(0);
	reset_hold((Window *) 0);
	hold_mode((Window *) 0, OFF, 1);
	if (current_screen->promptlist && current_screen->promptlist->type == WAIT_PROMPT_LINE)
	{
		OldPrompt = current_screen->promptlist;
		(*OldPrompt->func)(OldPrompt->data, get_input());
		set_input(empty_string);
		current_screen->promptlist = OldPrompt->next;
		new_free(&OldPrompt->data);
		new_free(&OldPrompt->prompt);
		new_free(&OldPrompt);
		change_input_prompt(-1);
	}
	else
	{
		u_char	*line,
			*tmp = NULL;

		line = get_input();
		malloc_strcpy(&tmp, line);

		if (do_hook(INPUT_LIST, "%s", tmp))
		{
			if (get_int_var(INPUT_ALIASES_VAR))
				parse_line(NULL, tmp, empty_string,
					1, 0, 0);
			else
				parse_line(NULL, tmp, NULL,
					1, 0, 0);
		}
		update_input(UPDATE_ALL);
		new_free(&tmp);
	}
	from_server = server;
}
