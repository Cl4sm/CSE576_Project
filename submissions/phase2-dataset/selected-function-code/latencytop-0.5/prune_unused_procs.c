void prune_unused_procs(void)
{
	GList *entry, *entry2;
	struct latency_line *line;
	struct process *proc;

	entry = g_list_first(procs);
	while (entry) {
		proc = entry->data;
		entry2 = g_list_next(entry);
		if (!proc->used) {
			while (proc->latencies) {
				entry2 = g_list_first(proc->latencies);
				line = entry2->data;
				free(line);
				proc->latencies = g_list_delete_link(proc->latencies, entry2);
			}
			free(proc);
			procs = g_list_delete_link(procs, entry);
		}
		entry = entry2;
	}
}
