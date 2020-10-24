void macro_global_removevar(const char *str, int len)
{
	struct substvar *sv;
	struct substvar *last = NULL;
	int status;

	status = pthread_mutex_lock(&table_mutex);
	if (status)
		fatal(status);

	sv = system_table;

	while (sv) {
		if (!strncmp(str, sv->def, len) && sv->def[len] == '\0')
			break;
		last = sv;
		sv = sv->next;
	}

	if (sv && !sv->readonly) {
		if (last)
			last->next = sv->next;
		else
			system_table = sv->next;
		if (sv->def)
			free(sv->def);
		if (sv->val)
			free(sv->val);
		free(sv);
	}

	status = pthread_mutex_unlock(&table_mutex);
	if (status)
		fatal(status);

	return;
}