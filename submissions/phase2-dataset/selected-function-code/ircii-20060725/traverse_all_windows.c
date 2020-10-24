traverse_all_windows(flag)
	int	*flag;
{
	static	Window	*which;
	static	Screen	*screen;
	static	char	visible = 1;
	int	foo = 1;

	/* First call, return the current window basically */
	if (*flag)
	{
		*flag = 0;
		visible = 1;
		if (!screen_list)
			return (Window *) 0;
		screen = screen_list;
		which = screen->window_list;
		if (which)
			return (which);
		else
			foo = 0;
	}

	/*
	 * foo is used to indicate the the current screen has no windows.
	 * This happens when we create a new screen..  either way, we want
	 * to go on to the next screen, so if foo isn't set, then if which
	 * is already null, return it again (this should never happen, if
	 * traverse_all_windows()'s is called properly), else move on to
	 * the next window
	 */
	if (foo)
	{
		if (!which)
			return (Window *) 0;
		else
			which = which->next;
	}

	if (!which)
	{
		while (screen)
		{
			screen = screen->next;
			if (screen && screen->alive)
				break;
		}
		if (screen)
			which = screen->window_list;
	}

	if (which)
		return (which);
	/* 
	 * Got to the end of the visible list..  so we do the invisible list..
	 * Should also mean, that we've got to the end of all the visible
	 * screen..
	 */
	if (visible)
	{
		visible = 0;
		which = invisible_list;
		return (which);
	}
	return ((Window *) 0);
}
