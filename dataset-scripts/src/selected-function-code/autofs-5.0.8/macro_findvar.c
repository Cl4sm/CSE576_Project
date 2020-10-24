const struct substvar *
macro_findvar(const struct substvar *table, const char *str, int len)
{
	const struct substvar *sv = system_table;
	const struct substvar *lv = table;
#ifdef ENABLE_EXT_ENV
	/* holds one env var */
	static struct substvar *lv_var;
	static char *value;
	char etmp[512];
#endif

	/* First try the passed in local table */
	while (lv) {
		if (!strncmp(str, lv->def, len) && lv->def[len] == '\0')
			return lv;
		lv = lv->next;
	}

	/* Then look in the system wide table */
	while (sv) {
		if (!strncmp(str, sv->def, len) && sv->def[len] == '\0')
			return sv;
		sv = sv->next;
	}

#ifdef ENABLE_EXT_ENV
	/* builtin and local map failed, try the $ENV */
	memcpy(etmp, str, len);
	etmp[len]='\0';

	if ((value=getenv(etmp)) != NULL) {
		lv_var = macro_addvar(table, str, len, value);
		return(lv_var);
	}
#endif

	return NULL;
}