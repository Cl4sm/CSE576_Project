static void *st_queue_handler(void *arg)
{
	struct list_head *head;
	struct list_head *p;
	int status, ret;

	st_mutex_lock();

	while (1) {
		/*
		 * If the state queue list is empty, wait until an
		 * entry is added.
		 */
		head = &state_queue;

		while (list_empty(head)) {
			status = pthread_cond_wait(&cond, &mutex);
			if (status)
				fatal(status);
		}

		p = head->next;
		while(p != head) {
			struct state_queue *task;

			task = list_entry(p, struct state_queue, list);
			p = p->next;

			if (task->cancel) {
				list_del(&task->list);
				free(task);
				continue;
			}

			task->busy = 1;

			ret = run_state_task(task);
			if (!ret) {
				list_del(&task->list);
				free(task);
			}
		}

		while (1) {
			signaled = 0;
			while (!signaled) {
				status = pthread_cond_wait(&cond, &mutex);
				if (status)
					fatal(status);
			}

			head = &state_queue;
			p = head->next;
			while (p != head) {
				struct state_queue *task, *next;

				task = list_entry(p, struct state_queue, list);
				p = p->next;

				/* Task may have been canceled before it started */
				if (!task->thid && task->cancel)
					goto remove;

				if (!task->busy) {
					/* Start a new task */
					task->busy = 1;

					ret = run_state_task(task);
					if (!ret)
						goto remove;
					continue;
				}

				/* Still starting up */
				if (!task->thid)
					continue;

				if (task->cancel) {
					pthread_cancel(task->thid);
					task->cancel = 0;
					continue;
				}

				/* Still busy */
				if (!task->done)
					continue;

remove:
				/* No more tasks for this queue */
				if (list_empty(&task->pending)) {
					list_del(&task->list);
					free(task);
					continue;
				}

				/* Next task */
				next = list_entry((&task->pending)->next,
							struct state_queue, pending);

				list_del(&task->list);
				free(task);

				list_del_init(&next->pending);
				list_add_tail(&next->list, head);
				if (p == head)
					p = head->next;
			}

			if (list_empty(head))
				break;
		}
	}
}