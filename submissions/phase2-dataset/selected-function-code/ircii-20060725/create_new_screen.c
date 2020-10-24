create_new_screen()
{
	Screen	*new = NULL,
		**list;
	static	int	refnumber = 0;

	for (list = &screen_list; *list; list = &((*list)->next))
	{
		if (!(*list)->alive)
		{
			new = *list;
			break;
		}
	}
	if (!new)
	{
		new = (Screen *) malloc(sizeof(Screen));
		new->screennum = ++refnumber;
		new->next = screen_list;
		if (screen_list)
			screen_list->prev = new;
		screen_list = new;
	}
	new->last_window_refnum = 1;
	new->window_list = NULL;
	new->window_list_end = NULL;
	new->cursor_window = NULL;
	new->current_window = NULL;
	new->visible_windows = 0;
	new->window_stack = NULL;
	new->meta1_hit = new->meta2_hit = new->meta3_hit = new->meta4_hit = 0;
	new->meta5_hit = new->meta6_hit = new->meta7_hit = new->meta8_hit = 0;
	new->quote_hit = new->digraph_hit = new->inside_menu = 0;
	new->fdout = 1;
	new->fpout = stdout;
	new->fdin = 0;
	new->fpin = stdin;
	new->alive = 1;
	new->promptlist = NULL;
	new->redirect_name = NULL;
	new->redirect_token = NULL;
	new->tty_name = (u_char *) 0;
	new->li = main_screen ? main_screen->li : 24;
	new->co = main_screen ? main_screen->co : 79;
	new->old_term_li = -1;
	new->old_term_co = -1;

	input_reset_screen(new);
	
	new->redirect_server = -1;
	last_input_screen = new;
	return new;
}
