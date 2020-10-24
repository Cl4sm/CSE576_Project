void macro_setenv(struct substvar *table)
{
	const struct substvar *sv = system_table;
	const struct substvar *lv = table;

	/*
	 * First set environment from global table, matching local
	 * variables will overwrite these.
	 */
	while (sv) {
		if (sv->def)
			setenv(sv->def, sv->val, 1);
		sv = sv->next;
	}

	/* Next set environment from the local table */
	while (lv) {
		if (lv->def)
			setenv(lv->def, lv->val, 1);
		lv = lv->next;
	}

	return;
}