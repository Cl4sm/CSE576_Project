void destroy_list(struct list_t **list)
{
	struct list_t *curr, *next;

	if (*list == NULL)
		return;

	curr = (*list)->next;
	while (curr) {
		next = curr->next;
		free(curr);
		curr = next;
	}

	curr = (*list);
	while (curr) {
		next = curr->prev;
		free(curr);
		curr = next;
	}

	*list = NULL;
}