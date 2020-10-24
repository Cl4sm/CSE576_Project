int warn_on_duplicates(vector uniques, char *str)
{
	char *tmp;
	int i;

	vector_foreach_slot(uniques, tmp, i) {
		if (!strcmp(str, tmp)) {
			condlog(1, "multipath.conf line %d, duplicate keyword: %s", line_nr, str);
			return 0;
		}
	}
	tmp = strdup(str);
	if (!tmp)
		return 1;
	if (!vector_alloc_slot(uniques)) {
		free(tmp);
		return 1;
	}
	vector_set_slot(uniques, tmp);
	return 0;
}
