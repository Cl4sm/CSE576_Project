static	void
pop_window()
{
	int	refnum;
	WindowStack *tmp;
	Window	*win;

	while (1)
	{
		if (current_screen->window_stack)
		{
			refnum = current_screen->window_stack->refnum;
			tmp = current_screen->window_stack->next;
			new_free(&current_screen->window_stack);
			current_screen->window_stack = tmp;
			if ((win = get_window_by_refnum((u_int)refnum)) != NULL)
			{
				if (!win->visible)
					show_window(win);
				else
					set_current_window(win);
				break;
			}
		}
		else
		{
			say("The window stack is empty!");
			break;
		}
	}
}
