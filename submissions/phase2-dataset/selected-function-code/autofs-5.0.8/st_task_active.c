static int st_task_active(struct autofs_point *ap, enum states state)
{
	struct list_head *head;
	struct list_head *p, *q;
	struct state_queue *task, *waiting;
	unsigned int active = 0;

	st_mutex_lock();

	head = &state_queue;

	list_for_each(p, head) {
		task = list_entry(p, struct state_queue, list);

		if (task->ap != ap)
			continue;

		if (task->state == state) {
			active = 1;
			break;
		}

		if (state == ST_ANY) {
			active = 1;
			break;
		}

		list_for_each(q, &task->pending) {
			waiting = list_entry(q, struct state_queue, pending);

			if (waiting->state == state) {
				active = 1;
				break;
			}

			if (state == ST_ANY) {
				active = 1;
				break;
			}
		}
	}

	st_mutex_unlock();

	return active;
}