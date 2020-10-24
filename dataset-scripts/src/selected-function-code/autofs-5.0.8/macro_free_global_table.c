void macro_free_global_table(void)
{
	struct substvar *sv;
	struct substvar *next;
	int status;

	status = pthread_mutex_lock(&table_mutex);
	if (status)
		fatal(status);

	sv = system_table;

	while (sv) {
		if (sv->readonly) {
			sv = sv->next;
			continue;
		}
		next = sv->next;
		if (sv->def)
			free(sv->def);
		if (sv->val)
			free(sv->val);
		free(sv);
		sv = next;
	}

	system_table = &sv_osvers;

	status = pthread_mutex_unlock(&table_mutex);
	if (status)
		fatal(status);

	return;
}