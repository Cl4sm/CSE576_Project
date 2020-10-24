show_stack()
{
	WindowStack *last = (WindowStack *) 0,
	    *tmp, *crap;
	Window	*win;
	int	flag = 1;
	int	len = 4;
	int	clen = get_int_var(CHANNEL_NAME_WIDTH_VAR);
	int	check_clen = clen == 0;

	while ((win = traverse_all_windows(&flag)) != NULL)
	{
		if (win->name && ((int) my_strlen(win->name) > len))
			len = my_strlen(win->name);
		if (check_clen == 0)
			continue;
		if (win->current_channel && ((int) my_strlen(win->current_channel) > clen))
			clen = my_strlen(win->current_channel);
	}
	say("Window stack:");
	tmp = current_screen->window_stack;
	while (tmp)
	{
		if ((win = get_window_by_refnum(tmp->refnum)) != NULL)
		{
			list_a_window(win, len, clen);
			tmp = tmp->next;
		}
		else
		{
			crap = tmp->next;
			new_free(&tmp);
			if (last)
				last->next = crap;
			else
				current_screen->window_stack = crap;
			tmp = crap;
		}
	}
}
