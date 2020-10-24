delete_other_windows()
{
	Window	*tmp,
		*cur,
		*next;

	cur = curr_scr_win;
	tmp = current_screen->window_list;
	while (tmp)
	{
		next = tmp->next;
		if (tmp != cur)
		{
			delete_window(tmp);
			update_all_windows();
		}
		tmp = next;
	}
}
