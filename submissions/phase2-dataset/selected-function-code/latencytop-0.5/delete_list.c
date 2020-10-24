{
	GList *entry, *entry2,*next;
	struct latency_line *line;
	struct process *proc;

	while (lines) {
		entry = g_list_first(lines);
		line = entry->data;
		free(line);
		lines = g_list_delete_link(lines, entry);
	}
	entry = g_list_first(allprocs);
	while (entry) {
		next = g_list_next(entry);
		proc = entry->data;
		while (proc->latencies) {
			entry2 = g_list_first(proc->latencies);
			line = entry2->data;
			free(line);
			proc->latencies = g_list_delete_link(proc->latencies, entry2);
		}
		proc->max = 0;
		if (!proc->exists) {
			free(proc);
			allprocs = g_list_delete_link(allprocs, entry);
		}
		entry = next;
	}
	g_list_free(procs);
	procs = NULL;
}
