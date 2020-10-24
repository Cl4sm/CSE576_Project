{
	int r;
	struct event_thread *waiter;

	mlockall(MCL_CURRENT | MCL_FUTURE);

	waiter = (struct event_thread *)et;
	pthread_cleanup_push(free_waiter, et);

	while (1) {
		r = waiteventloop(waiter);

		if (r < 0)
			break;

		sleep(r);
	}

	pthread_cleanup_pop(1);
	return NULL;
}
