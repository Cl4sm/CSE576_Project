term_resize()
{

	/*
	 * if we're not the main screen, we've probably arrived here via
	 * the wserv message path, and we should have already setup the
	 * values of "li" and "co".
	 */
	if (is_main_screen(current_screen))
	{
#ifndef TIOCGWINSZ
		current_screen->li = li;
		current_screen->co = co;
#else
		struct	winsize window;

		if (ioctl(tty_des, TIOCGWINSZ, &window) < 0)
		{
			current_screen->li = li;
			current_screen->co = co;
		}
		else
		{
			if ((current_screen->li = window.ws_row) == 0)
				current_screen->li = li;
			if ((current_screen->co = window.ws_col) == 0)
				current_screen->co = co;
		}
#endif /* TIOCGWINSZ */
		current_screen->co--;
	}
	if ((current_screen->old_term_li != current_screen->li) || (current_screen->old_term_co != current_screen->co))
	{
		current_screen->old_term_li = current_screen->li;
		current_screen->old_term_co = current_screen->co;
		return (1);
	}
	return (0);
}
