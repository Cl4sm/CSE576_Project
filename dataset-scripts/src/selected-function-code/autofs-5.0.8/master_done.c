int master_done(struct master *master)
{
	struct list_head *head, *p;
	struct master_mapent *entry;
	int res = 0;

	head = &master->completed;
	p = head->next;
	while (p != head) {
		entry = list_entry(p, struct master_mapent, join);
		p = p->next;
		list_del(&entry->join);
		pthread_join(entry->thid, NULL);
		master_free_mapent_sources(entry, 1);
		master_free_mapent(entry);
	}
	if (list_empty(&master->mounts))
		res = 1;

	return res;
}