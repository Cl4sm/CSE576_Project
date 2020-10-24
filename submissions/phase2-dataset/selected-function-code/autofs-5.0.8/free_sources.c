int free_sources(struct list_head *list)
{
	struct nss_source *this;
	struct list_head *head, *next;

	if (list_empty(list))
		return 0;

	head = list;
	next = list->next;
	while (next != head) {
		this = list_entry(next, struct nss_source, list);
		next = next->next;

		list_del(&this->list);
		if (this->source)
			free(this->source);
		free(this);
	}
	return 1;
}