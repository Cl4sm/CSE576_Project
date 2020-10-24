{
	GList *entry;
	struct process *proc;

	entry = g_list_first(allprocs);
	while (entry) {
		proc = entry->data;
		if (proc->pid == pid) {
			return proc;
		}
		entry = g_list_next(entry);
	}
	proc = malloc(sizeof(struct process));
	memset(proc, 0, sizeof(struct process));
	proc->pid = pid;
	allprocs = g_list_append(allprocs, proc);

	return proc;
}
