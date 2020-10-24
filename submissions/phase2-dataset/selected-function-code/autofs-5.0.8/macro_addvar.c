struct substvar *
macro_addvar(struct substvar *table, const char *str, int len, const char *value)
{
	struct substvar *lv = table;

	while (lv) {
		if (!strncmp(str, lv->def, len) && lv->def[len] == '\0')
			break;
		lv = lv->next;
	}

	if (lv) {
		char *this = realloc(lv->val, strlen(value) + 1);
		if (!this) {
			lv = table;
			goto done;
		}
		strcat(this, value);
		lv->val = this;
	} else {
		struct substvar *new;
		char *def, *val;

		def = strdup(str);
		if (!def) {
			lv = table;
			goto done;
		}
		def[len] = '\0';

		val = strdup(value);
		if (!val) {
			lv = table;
			free(def);
			goto done;
		}

		new = malloc(sizeof(struct substvar));
		if (!new) {
			lv = table;
			free(def);
			free(val);
			goto done;
		}
		new->def = def;
		new->val = val;
		new->readonly = 0;
		new->next = table;
		lv = new;
	}
done:

	return lv;
}