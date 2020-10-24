struct master_mapent *master_new_mapent(struct master *master, const char *path, time_t age)
{
	struct master_mapent *entry;
	int status;
	char *tmp;

	entry = malloc(sizeof(struct master_mapent));
	if (!entry)
		return NULL;

	memset(entry, 0, sizeof(struct master_mapent));

	tmp = strdup(path);
	if (!tmp) {
		free(entry);
		return NULL;
	}
	entry->path = tmp;

	entry->thid = 0;
	entry->age = age;
	entry->master = master;
	entry->current = NULL;
	entry->maps = NULL;
	entry->ap = NULL;

	status = pthread_rwlock_init(&entry->source_lock, NULL);
	if (status)
		fatal(status);

	status = pthread_mutex_init(&entry->current_mutex, NULL);
	if (status)
		fatal(status);

	status = pthread_cond_init(&entry->current_cond, NULL);
	if (status)
		fatal(status);

	INIT_LIST_HEAD(&entry->list);

	return entry;
}