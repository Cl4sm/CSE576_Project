irc_goto_window(which)
	int	which;
{
	Window	*tmp;
	int	i;


	if (which == 0)
		return;
	if ((which < 0) || (which > current_screen->visible_windows))
	{
		say("GOTO: Illegal value");
		return;
	}
	tmp = current_screen->window_list;
	for (i = 1; tmp && (i != which); tmp = tmp->next, i++)
		;
	set_current_window(tmp);
}
