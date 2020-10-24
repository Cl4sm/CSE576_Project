static report_file(char *process, char *file)
{
	struct fsync_process *proc;
	GList *item;

	item = fsync_data;
	while (item) {
		proc = item->data;
		item = g_list_next(item);
		if (strcmp(proc->name, process) == 0) {
			chain_file(proc, file);
			return;
		}
	}

	proc = malloc(sizeof(struct fsync_process));
	if (!proc)
		return;
	memset(proc, 0, sizeof(struct fsync_process));
	strncpy(proc->name, process, PATH_MAX-1);
	chain_file(proc, file);
	fsync_data = g_list_append(fsync_data, proc);
}
