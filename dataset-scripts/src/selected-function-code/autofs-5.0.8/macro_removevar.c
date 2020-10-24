struct substvar *
macro_removevar(struct substvar *table, const char *str, int len)
{
	struct substvar *list, *lv;
	struct substvar *last = NULL;

	lv = list = table;

	while (lv) {
		if (!strncmp(str, lv->def, len) && lv->def[len] == '\0')
			break;
		last = lv;
		lv = lv->next;
	}

	if (lv) {
		if (last)
			last->next = lv->next;
		else
			list = lv->next;
		if (lv->def)
			free(lv->def);
		if (lv->val)
			free(lv->val);
		free(lv);
	}

	return list;
}