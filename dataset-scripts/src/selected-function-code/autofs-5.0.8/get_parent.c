static struct mapent *get_parent(const char *key, struct list_head *head, struct list_head **pos)
{
	struct list_head *next;
	struct mapent *this, *last;
	int eq;

	last = NULL;
	next = *pos ? (*pos)->next : head->next;

	list_for_each(next, head) {
		this = list_entry(next, struct mapent, multi_list);

		if (!strcmp(this->key, key))
			break;

		eq = strncmp(this->key, key, strlen(this->key));
		if (eq == 0) {
			*pos = next;
			last = this;
			continue;
		}
	}

	return last;
}