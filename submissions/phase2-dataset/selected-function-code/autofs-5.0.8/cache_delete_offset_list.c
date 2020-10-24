int cache_delete_offset_list(struct mapent_cache *mc, const char *key)
{
	unsigned logopt = mc->ap ? mc->ap->logopt : master_get_logopt();
	struct mapent *me;
	struct mapent *this;
	struct list_head *head, *next;
	int remain = 0;
	int status;

	me = cache_lookup_distinct(mc, key);
	if (!me)
		return CHE_FAIL;

	/* Not offset list owner */
	if (me->multi != me)
		return CHE_FAIL;

	head = &me->multi_list;
	next = head->next;
	while (next != head) {
		this = list_entry(next, struct mapent, multi_list);
		next = next->next;
		if (this->ioctlfd != -1) {
			error(logopt,
			      "active offset mount key %s", this->key);
			return CHE_FAIL;
		}
	}

	head = &me->multi_list;
	next = head->next;
	while (next != head) {
		this = list_entry(next, struct mapent, multi_list);
		next = next->next;
		list_del_init(&this->multi_list);
		this->multi = NULL;
		debug(logopt, "deleting offset key %s", this->key);
		status = cache_delete(mc, this->key);
		if (status == CHE_FAIL) {
			warn(logopt,
			     "failed to delete offset %s", this->key);
			this->multi = me;
			/* TODO: add list back in */
			remain++;
		}
	}

	if (!remain) {
		list_del_init(&me->multi_list);
		me->multi = NULL;
	}

	if (remain)
		return CHE_FAIL;

	return CHE_OK;
}