update_all_windows()
{
	Window	*tmp;
	int	fast_window,
		full_window,
		r_status,
		u_status;

	for (tmp = current_screen->window_list; tmp; tmp = tmp->next)
	{
		if (tmp->display_size != tmp->old_size)
			resize_display(tmp);
		if (tmp->update)
		{
			fast_window = tmp->update & REDRAW_DISPLAY_FAST;
			full_window = tmp->update & REDRAW_DISPLAY_FULL;
			r_status = tmp->update & REDRAW_STATUS;
			u_status = tmp->update & UPDATE_STATUS;
			if (full_window)
				redraw_window(tmp, 1, 0);
			else if (fast_window)
				redraw_window(tmp, 0, 0);
			if (r_status)
				update_window_status(tmp, 1);
			else if (u_status)
				update_window_status(tmp, 0);
		}
		tmp->update = 0;
	}
	for (tmp = invisible_list; tmp; tmp = tmp->next)
	{
		if (tmp->display_size != tmp->old_size)
			resize_display(tmp);
		tmp->update = 0;
	}
	update_input(UPDATE_JUST_CURSOR);
	term_flush();
}
