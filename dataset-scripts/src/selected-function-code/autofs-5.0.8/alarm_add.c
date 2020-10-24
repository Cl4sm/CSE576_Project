int alarm_add(struct autofs_point *ap, time_t seconds)
{
	struct list_head *head;
	struct list_head *p;
	struct alarm *new;
	time_t now = time(NULL);
	time_t next_alarm = 0;
	unsigned int empty = 1;
	int status;

	if (!seconds)
		return 1;

	new = malloc(sizeof(struct alarm));
	if (!new)
		return 0;

	new->ap = ap;
	new->cancel = 0;
	new->time = now + seconds;

	alarm_lock();

	head = &alarms;

	/* Check if we have a pending alarm */
	if (!list_empty(head)) {
		struct alarm *current;
		current = list_entry(head->next, struct alarm, list);
		next_alarm = current->time;
		empty = 0;
	}

	list_for_each(p, head) {
		struct alarm *this;

		this = list_entry(p, struct alarm, list);
		if (this->time >= new->time) {
			list_add_tail(&new->list, p);
			break;
		}
	}
	if (p == head)
		list_add_tail(&new->list, p);

	/*
	 * Wake the alarm thread if it is not busy (ie. if the
	 * alarms list was empty) or if the new alarm comes before
	 * the alarm we are currently waiting on.
	 */
	if (empty || new->time < next_alarm) {
        	status = pthread_cond_signal(&cond);
		if (status)
			fatal(status);
	}

	alarm_unlock();

	return 1;
}