int __st_add_task(struct autofs_point *ap, enum states state)
{
	struct list_head *head;
	struct list_head *p, *q;
	struct state_queue *new;
	unsigned int empty = 1;
	int status;

	/* Task termination marker, poke state machine */
	if (state == ST_READY) {
		st_ready(ap);

		signaled = 1;
		status = pthread_cond_signal(&cond);
		if (status)
			fatal(status);

		return 1;
	}

	if (ap->state == ST_SHUTDOWN)
		return 1;

	if (state == ST_SHUTDOWN)
		return st_shutdown(ap);

	head = &state_queue;

	/* Add to task queue for autofs_point ? */
	list_for_each(p, head) {
		struct state_queue *task;

		task = list_entry(p, struct state_queue, list);

		if (task->ap != ap)
			continue;

		empty = 0;

		/* Don't add duplicate tasks */
		if ((task->state == state && !task->done) ||
		   (ap->state == ST_SHUTDOWN_PENDING ||
		    ap->state == ST_SHUTDOWN_FORCE))
			break;

		/* No pending tasks */
		if (list_empty(&task->pending)) {
			new = st_alloc_task(ap, state);
			if (new)
				list_add_tail(&new->pending, &task->pending);
			goto done;
		}

		list_for_each(q, &task->pending) {
			struct state_queue *p_task;

			p_task = list_entry(q, struct state_queue, pending);

			if (p_task->state == state ||
			   (ap->state == ST_SHUTDOWN_PENDING ||
			    ap->state == ST_SHUTDOWN_FORCE))
				goto done;
		}

		new = st_alloc_task(ap, state);
		if (new)
			list_add_tail(&new->pending, &task->pending);
done:
		break;
	}

	if (empty) {
		new = st_alloc_task(ap, state);
		if (new)
			list_add(&new->list, head);
		/* Added to empty state queue, kick state machine */
		signaled = 1;
		status = pthread_cond_signal(&cond);
		if (status)
			fatal(status);
	}

	return 1;
}