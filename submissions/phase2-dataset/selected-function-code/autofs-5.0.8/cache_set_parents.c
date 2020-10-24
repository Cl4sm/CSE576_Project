int cache_set_parents(struct mapent *mm)
{
	struct list_head *multi_head, *p, *pos;
	struct mapent *this;

	if (!mm->multi)
		return 0;

	pos = NULL;
	multi_head = &mm->multi->multi_list;

	list_for_each(p, multi_head) {
		struct mapent *parent;
		this = list_entry(p, struct mapent, multi_list);
		parent = get_parent(this->key, multi_head, &pos);
		if (parent)
			this->parent = parent;
		else
			this->parent = mm->multi;
	}

	return 1;
}