int insert_sorted_if_unique(struct list_t **list, char const *const name)
{
	struct list_t *prev, *curr, *next;
	int cmp;

	if (*list == NULL)
		return update_list(list, NULL, NULL, name);

	curr = NULL;
	next = *list;
	do {
		prev = curr;
		curr = next;
		cmp = strcmp(name, curr->name);
		if (cmp == 0)
			return 1;
		else if (cmp > 0)
			next = curr->next;
		else
			next = curr->prev;
	} while (next && next != prev);

	if (cmp > 0)
		return update_list(list, curr, curr->next, name);
	else
		return update_list(list, curr->prev, curr, name);
}