int macro_is_systemvar(const char *str, int len)
{
	struct substvar *sv;
	int found = 0;
	int status;

	status = pthread_mutex_lock(&table_mutex);
	if (status)
		fatal(status);

	sv = system_table;

	while (sv) {
		if (!strncmp(str, sv->def, len) && sv->def[len] == '\0') {
			found = 1;
			break;
		}
		sv = sv->next;
	}

	status = pthread_mutex_unlock(&table_mutex);
	if (status)
		fatal(status);

	return found;
}