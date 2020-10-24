static void print_process(unsigned int pid)
{
	struct process *proc;
	GList *item;
	werase(right_window);
	double total = 0.0;

	item = g_list_first(procs);
	while (item) {
		char header[4096];
		int i = 0;
		GList *item2;
		struct latency_line *line;
		proc = item->data;
		item = g_list_next(item);
		if (proc->pid != pid)
			continue;
		wattron(right_window, A_REVERSE);
		sprintf(header, "Process %s (%i) ", proc->name, proc->pid);
		while (strlen(header) < maxx)
			strcat(header, " ");
		mvwprintw(right_window, 0, 0, "%s", header);
		
		item2 = g_list_first(proc->latencies);
		while (item2 && i < 6) {
			line = item2->data;
			item2 = g_list_next(item2);
			total = total + line->time;
		}
		mvwprintw(right_window, 0, 43, "Total: %5.1f msec", total*0.001);
		wattroff(right_window, A_REVERSE);
		item2 = g_list_first(proc->latencies);
		while (item2 && i < 6) {
			line = item2->data;
			item2 = g_list_next(item2);
			if (line->max*0.001 < 0.1)
				continue;
			mvwprintw(right_window, i+1, 0, "%s", line->reason);
			mvwprintw(right_window, i+1, 50, "%5.1f msec        %5.1f %%",
				line->max * 0.001,
				(line->time * 100 +0.0001) / total
				);
			i++;
		}

	}
	wrefresh(right_window);
}
