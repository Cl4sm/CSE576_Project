{
	GList *entry, *start = NULL;
	struct process *proc;
	char startswith;

	entry = procs;
	while (entry) {
		proc = entry->data;
		if (proc->pid == cursor_pid) {
			start = entry;
			break;
		}	
		entry = g_list_next(entry);
	}
	while (start) {
		if (g_list_next(start))
				start = g_list_next(start);
		if (start) {
			proc=start->data;
			startswith = proc->name[0];
			startswith = toupper (startswith);
			if ((filter == '\0') || (startswith == filter))
				return proc->pid;
			else
				start = g_list_next(start);
		}
	}
	return 0;
}
