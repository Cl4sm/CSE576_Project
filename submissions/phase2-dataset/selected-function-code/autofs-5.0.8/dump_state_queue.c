void dump_state_queue(void)
{
	struct list_head *head = &state_queue;
	struct list_head *p, *q;

	logmsg("dumping queue");

	list_for_each(p, head) {
		struct state_queue *entry;

		entry = list_entry(p, struct state_queue, list);
		logmsg("queue list head path %s state %d busy %d",
		      entry->ap->path, entry->state, entry->busy);

		list_for_each(q, &entry->pending) {
			struct state_queue *this;

			this = list_entry(q, struct state_queue, pending);
			logmsg("queue list entry path %s state %d busy %d",
			      this->ap->path, this->state, this->busy);
		}
	}
}