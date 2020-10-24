{
	GList *entry, *start = NULL;
	struct process *proc;
	int i = 0, xpos = 0;
	char startswith;

	entry = procs;
	
retry:
	werase(process_window);
	xpos = 0;
	start = cursor_e;	
	if (!start) {
		start = g_list_first(procs);
		cursor_e = start;
	}
	
	if (!start)
		return;	
		
	proc = start->data;
	while (proc->pid > cursor_pid && cursor_pid > 0) {
		start = g_list_previous(start);
		proc = start->data;
		cursor_e = start;
	}

	/* and print 7 */
	i = 0;
	while (start) {
		proc = start->data;	
		startswith = proc->name[0];
		startswith = toupper(startswith);
		if ((filter != '\0') && (startswith != filter)) {
			start = g_list_next(start);
			continue;
		}

		if (proc->pid == cursor_pid) {
			if (xpos + strlen(proc->name) + 2 > maxx && cursor_e) {
				cursor_e = g_list_next(cursor_e);
				goto retry;
			}
			wattron(process_window, A_REVERSE);
		}
		
		if (xpos + strlen(proc->name) + 2 <= maxx) 
			mvwprintw(process_window, 0, xpos, " %s ", proc->name);
		xpos += strlen(proc->name)+2;
		
		wattroff(process_window, A_REVERSE);

		start = g_list_next(start);

		i++;
	}	
	wrefresh(process_window);
}
