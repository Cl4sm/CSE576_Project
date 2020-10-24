Window	*
new_window()
{
	Window	*new;
	static	int	no_screens = 1;

	if (no_screens)
	{
		set_current_screen(create_new_screen());
		main_screen = current_screen;
		no_screens = 0;
	}
	if (dumb && (current_screen->visible_windows == 1))
		return NULL;
	new = (Window *) new_malloc(sizeof(Window));
	new->refnum = create_refnum();
	if (curr_scr_win)
		new->server = curr_scr_win->server;
	else
		new->server = primary_server;
	new->prev_server = -1;
	new->line_cnt = 0;
	if (current_screen->visible_windows == 0)
		new->window_level = LOG_DEFAULT;
	else
		new->window_level = LOG_NONE;
	new->hold_mode = get_int_var(HOLD_MODE_VAR);
	new->scroll = get_int_var(SCROLL_VAR);
	new->lastlog_head = 0;
	new->lastlog_tail = 0;
	new->nicks = 0;
	new->lastlog_level = real_lastlog_level();
	new->name = 0;
	new->prompt = 0;
	new->lastlog_size = 0;
	new->held = OFF;
	new->last_held = OFF;
	new->current_channel = 0;
	new->bound_channel = 0;
	new->query_nick = 0;
	new->hold_on_next_rite = 0;
	new->status_line[0] = NULL;
	new->status_line[1] = NULL;
	new->double_status = 0;
	new->top_of_display = 0;
	new->display_ip = 0;
	new->display_size = 1;
	new->old_size = 1;
	new->hold_head = 0;
	new->hold_tail = 0;
	new->held_lines = 0;
	new->scrolled_lines = 0;
	new->new_scrolled_lines = 0;
	new->next = 0;
	new->prev = 0;
	new->cursor = 0;
	new->visible = 1;
	new->screen = current_screen;
	new->logfile = 0;
	new->log = 0;
	new->log_fp = 0;
	new->miscflags = 0;
	new->update = 0;
	new->menu.lines = 0;
	new->menu.menu = 0;
	new->notify_level = real_notify_level();
	new->server_group = 0;
	new->sticky = 1;
	resize_display(new);
	if (add_to_window_list(new))
		set_current_window(new);
	term_flush();
	return (new);
}
