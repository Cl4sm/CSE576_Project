static void cache_add_ordered_offset(struct mapent *me, struct list_head *head)
{
	struct list_head *p;
	struct mapent *this;

	list_for_each(p, head) {
		size_t tlen;
		int eq;

		this = list_entry(p, struct mapent, multi_list);
		tlen = strlen(this->key);

		eq = strncmp(this->key, me->key, tlen);
		if (!eq && tlen == strlen(me->key))
			return;

		if (eq > 0) {
			list_add_tail(&me->multi_list, p);
			return;
		}
	}
	list_add_tail(&me->multi_list, p);

	return;
}