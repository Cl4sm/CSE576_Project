void master_free_mapent(struct master_mapent *entry)
{
	int status;

	if (entry->path)
		free(entry->path);

	master_free_autofs_point(entry->ap);

	status = pthread_rwlock_destroy(&entry->source_lock);
	if (status)
		fatal(status);

	status = pthread_mutex_destroy(&entry->current_mutex);
	if (status)
		fatal(status);

	status = pthread_cond_destroy(&entry->current_cond);
	if (status)
		fatal(status);

	free(entry);

	return;
}