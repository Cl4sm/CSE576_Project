int macro_global_addvar(const char *str, int len, const char *value)
{
	struct substvar *sv;
	int status, ret = 0;

	status = pthread_mutex_lock(&table_mutex);
	if (status)
		fatal(status);

	sv = system_table;

	while (sv) {
		if (!strncmp(str, sv->def, len) && sv->def[len] == '\0')
			break;
		sv = sv->next;
	}

	if (sv && !sv->readonly) {
		char *this = realloc(sv->val, strlen(value) + 1);
		if (!this)
			goto done;
		strcat(this, value);
		sv->val = this;
		ret = 1;
	} else {
		struct substvar *new;
		char *def, *val;

		def = strdup(str);
		if (!def)
			goto done;
		def[len] = '\0';

		val = strdup(value);
		if (!val) {
			free(def);
			goto done;
		}

		new = malloc(sizeof(struct substvar));
		if (!new) {
			free(def);
			free(val);
			goto done;
		}
		new->def = def;
		new->val = val;
		new->readonly = 0;
		new->next = system_table;
		system_table = new;
		ret =1;
	}
done:
	status = pthread_mutex_unlock(&table_mutex);
	if (status)
		fatal(status);

	return ret;
}