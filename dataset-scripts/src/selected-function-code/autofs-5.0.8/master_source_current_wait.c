void master_source_current_wait(struct master_mapent *entry)
{
	int status;

	status = pthread_mutex_lock(&entry->current_mutex);
	if (status) {
		logmsg("entry current source lock failed");
		fatal(status);
	}

	while (entry->current != NULL) {
		status = pthread_cond_wait(
				&entry->current_cond, &entry->current_mutex);
		if (status) {
			logmsg("entry current source condition wait failed");
			fatal(status);
		}
	}

	return;
}