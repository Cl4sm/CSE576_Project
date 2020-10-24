static	void
scrollback_forwards_lines(ScrollDist)
	int	ScrollDist;
{
	Window	*window;

	Debug((3, "scrollback_forward_lines(%d)", ScrollDist));
	window = curr_scr_win;
	if (!window->scrolled_lines)
	{
		term_beep();
		return;
	}
	if (ScrollDist > window->scrolled_lines)
		ScrollDist = window->scrolled_lines;

	Debug((3, "scrolled_lines = %d", window->scrolled_lines));
	window->scrolled_lines -= ScrollDist;
	Debug((3, "going to term_scroll(%d, %d, %d)",
	    window->top + window->menu.lines,
	    window->top + window->menu.lines + window->display_size - 1,
	    ScrollDist));
	term_scroll(window->top + window->menu.lines,
	    window->top + window->menu.lines + window->display_size - 1,
	    ScrollDist);

	Debug((3, "scrolled_lines=%d, new_scrolled_lines=%d, display_size=%d",
	    window->scrolled_lines,
	    window->new_scrolled_lines,
	    window->display_size));

	if (window->scrolled_lines < window->display_size)
		redraw_window(window,
		    ScrollDist + window->scrolled_lines - window->display_size, 1);

	Debug((3, "going to display_lastlog_lines(%d, %d, %s)",
	    window->scrolled_lines - window->display_size,
	    window->scrolled_lines - window->display_size + ScrollDist,
	    window->name));
	display_lastlog_lines(window->scrolled_lines - window->display_size,
	    window->scrolled_lines - window->display_size + ScrollDist,
	    window);
	cursor_not_in_display();
	update_input(UPDATE_JUST_CURSOR);

	if (!window->scrolled_lines)
	{
		window->new_scrolled_lines = 0;
		if (window->hold_mode)
			hold_mode(window, ON, 1);
		else
			hold_mode(window, OFF, 0);
	}
	window->update |= UPDATE_STATUS;
	update_window_status(window, 0);
}
