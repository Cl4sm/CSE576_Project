void st_remove_tasks(struct autofs_point *ap)
{
	struct list_head *head;
	struct list_head *p, *q;
	struct state_queue *task, *waiting;
	int status;

	st_mutex_lock();

	head = &state_queue;

	if (list_empty(head)) {
		st_mutex_unlock();
		return;
	}

	p = head->next;
	while (p != head) {
		task = list_entry(p, struct state_queue, list);
		p = p->next;

		if (task->ap != ap)
			continue;

		if (task->busy) {
			/* We only cancel readmap, prune and expire */
			if (task->state == ST_EXPIRE ||
			    task->state == ST_PRUNE ||
			    task->state == ST_READMAP)
				task->cancel = 1;
		}

		q = (&task->pending)->next;
		while(q != &task->pending) {
			waiting = list_entry(q, struct state_queue, pending);
			q = q->next;

			/* Don't remove existing shutdown task */
			if (waiting->state != ST_SHUTDOWN_PENDING &&
			    waiting->state != ST_SHUTDOWN_FORCE) {
				list_del(&waiting->pending);
				free(waiting);
			}
		}
	}

	signaled = 1;
	status = pthread_cond_signal(&cond);
	if (status)
		fatal(status);

	st_mutex_unlock();

	return;
}