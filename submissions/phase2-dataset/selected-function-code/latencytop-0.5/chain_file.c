static chain_file(struct fsync_process *proc, char *filename)
{
	struct fsync_files *file;
	GList *item;

	proc->fsync_count++;
	item = proc->files;
	while (item) {
		file = item->data;
		item = g_list_next(item);
		if (strcmp(file->name, filename)==0) {
			file->fsync_count++;
			return;
		}
	}
	file = malloc(sizeof(struct fsync_files));
	if (!file)
		return;
	memset(file, 0, sizeof(struct fsync_files));
	strncpy(file->name, filename, PATH_MAX-1);
	file->fsync_count = 1;
	proc->files = g_list_append(proc->files, file);
}
