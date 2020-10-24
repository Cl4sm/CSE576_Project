recalculate_windows()
{
	int	base_size,
	size,
	top,
	extra;
	Window	*tmp;
	
	if (dumb || current_screen->visible_windows == 0)
		return;

	base_size = ((current_screen->li - 1) / current_screen->visible_windows) - 1;
	extra = (current_screen->li - 1) - ((base_size + 1)*current_screen->visible_windows);
	top = 0;
	for (tmp = current_screen->window_list; tmp; tmp = tmp->next)
	{
		tmp->update |= REDRAW_DISPLAY_FULL | REDRAW_STATUS;
		if (extra)
		{
			extra--;
			size = base_size + 1;
		}
		else
			size = base_size;
#ifdef SCROLL_AFTER_DISPLAY
		tmp->display_size = size - tmp->menu.lines
			- tmp->double_status - 1;
#else
		tmp->display_size = size - tmp->menu.lines
			- tmp->double_status;
#endif /* SCROLL_AFTER_DISPLAY */
		if (tmp->display_size<=0)
			tmp->display_size = 1;
		tmp->top = top;
		tmp->bottom = top + size - tmp->double_status;
		top += size + 1;
	}
}
