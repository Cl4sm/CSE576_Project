static void *alarm_handler(void *arg)
{
	struct list_head *head;
	struct timespec expire;
	struct alarm *first;
	time_t now;
	int status;

	alarm_lock();

	head = &alarms;

	while (1) {
		if (list_empty(head)) {
			/* No alarms, wait for one to be added */
			status = pthread_cond_wait(&cond, &mutex);
			if (status)
				fatal(status);
			continue;
		}

		first = list_entry(head->next, struct alarm, list);

		now = time(NULL);

		if (first->time > now) {
			struct timeval usecs;
			/* 
			 * Wait for alarm to trigger or a new alarm 
			 * to be added.
			 */
			gettimeofday(&usecs, NULL);
			expire.tv_sec = first->time;
			expire.tv_nsec = usecs.tv_usec * 1000;

			status = pthread_cond_timedwait(&cond, &mutex, &expire);
			if (status && status != ETIMEDOUT)
				fatal(status);
		} else {
			/* First alarm has triggered, run it */

			list_del(&first->list);

			if (!first->cancel) {
				struct autofs_point *ap = first->ap;
				alarm_unlock(); 
				st_add_task(ap, ST_EXPIRE);
				alarm_lock();
			}
			free(first);
		}
	}
	/* Will never come here, so alarm_unlock is not necessary */
}