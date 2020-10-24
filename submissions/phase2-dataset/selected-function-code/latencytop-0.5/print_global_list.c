static void print_global_list(void)
{
	GList *item;
	struct latency_line *line;
	int i = 1;

	mvwprintw(global_window, 0, 0, "Cause");
	mvwprintw(global_window, 0, 50, "   Maximum     Percentage\n");
	item = g_list_first(lines);
	while (item && i < 10) {
		line = item->data;
		item = g_list_next(item);
		
		if (line->max*0.001 < 0.1) 
			continue;
		mvwprintw(global_window, i, 0, "%s", line->reason);
		mvwprintw(global_window, i, 50, "%5.1f msec        %5.1f %%\n",
				line->max * 0.001,
				(line->time * 100 +0.0001) / total_time);
		i++;
	}
	wrefresh(global_window);

}
