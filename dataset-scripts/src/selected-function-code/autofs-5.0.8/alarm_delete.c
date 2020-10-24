void alarm_delete(struct autofs_point *ap)
{
	struct list_head *head;
	struct list_head *p;
	struct alarm *current;
	unsigned int signal_cancel = 0;
	int status;

	alarm_lock();

	head = &alarms;

	if (list_empty(head)) {
		alarm_unlock();
		return;
	}

	current = list_entry(head->next, struct alarm, list);

	p = head->next;
	while (p != head) {
		struct alarm *this;

		this = list_entry(p, struct alarm, list);
		p = p->next;

		if (ap == this->ap) {
			if (current != this) {
				list_del_init(&this->list);
				free(this);
				continue;
			}
			/* Mark as canceled */
			this->cancel = 1;
			this->time = 0;
			signal_cancel = 1;
		}
	}

	if (signal_cancel) {
        	status = pthread_cond_signal(&cond);
		if (status)
			fatal(status);
	}

	alarm_unlock();

	return;
}