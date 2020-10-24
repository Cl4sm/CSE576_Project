set_scroll(value)
	int	value;
{
	if (value && (get_int_var(SCROLL_LINES_VAR) == 0))
	{
		put_it("You must set SCROLL_LINES to a positive value first!");
		if (curr_scr_win)
			curr_scr_win->scroll = 0;
	}
	else
	{
		if (curr_scr_win)
		{
			int	old_value = curr_scr_win->scroll;

			curr_scr_win->scroll = value;
			if (old_value != value)
				scroll_window(curr_scr_win);
		}
	}
}
