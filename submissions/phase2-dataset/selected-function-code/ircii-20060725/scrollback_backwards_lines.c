scrollback_backwards_lines(ScrollDist)
	int	ScrollDist;
{
	Window	*window;
	int totallines;
	
	Debug((3, "scrollback_backwards_lines(%d)", ScrollDist));
	window = curr_scr_win;
	if (!window->scrolled_lines && !window->scroll)
	{
		term_beep();
		return;
	}
	totallines = lastlog_lines(window);
	Debug((3, "totallines = %d, scrolled_lines = %d", totallines, window->scrolled_lines));
	if (ScrollDist + window->scrolled_lines > totallines)
	{
		ScrollDist = totallines - window->scrolled_lines;
		Debug((3, "  adjusting ScrollDist to %d", ScrollDist));
	}
	if (ScrollDist == 0)
	{
		term_beep();
		return;
	}
	window->scrolled_lines += ScrollDist;

	Debug((3, "going to term_scroll(%d, %d, %d)",
	    window->top + window->menu.lines,
	    window->top + window->menu.lines + window->display_size - 1,
	    -ScrollDist));
	term_scroll(window->top + window->menu.lines,
	    window->top + window->menu.lines + window->display_size - 1,
	    -ScrollDist);

	Debug((3, "scrolled_lines=%d, new_scrolled_lines=%d, display_size=%d",
	    window->scrolled_lines,
	    window->new_scrolled_lines,
	    window->display_size));

	Debug((3, "going to display_lastlog_lines(%d, %d, %s)",
	    window->scrolled_lines - ScrollDist,
	    window->scrolled_lines,
	    window->name));
	display_lastlog_lines(window->scrolled_lines - ScrollDist,
	    window->scrolled_lines,
	    window);
	cursor_not_in_display();
	update_input(UPDATE_JUST_CURSOR);
	window->update |= UPDATE_STATUS;
	update_window_status(window, 0);
}
