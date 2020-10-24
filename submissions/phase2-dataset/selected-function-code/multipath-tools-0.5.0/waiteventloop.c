{
	sigset_t set, oldset;
	int event_nr;
	int r;

	if (!waiter->event_nr)
		waiter->event_nr = dm_geteventnr(waiter->mapname);

	if (!(waiter->dmt = dm_task_create(DM_DEVICE_WAITEVENT))) {
		condlog(0, "%s: devmap event #%i dm_task_create error",
				waiter->mapname, waiter->event_nr);
		return 1;
	}

	if (!dm_task_set_name(waiter->dmt, waiter->mapname)) {
		condlog(0, "%s: devmap event #%i dm_task_set_name error",
				waiter->mapname, waiter->event_nr);
		dm_task_destroy(waiter->dmt);
		waiter->dmt = NULL;
		return 1;
	}

	if (waiter->event_nr && !dm_task_set_event_nr(waiter->dmt,
						      waiter->event_nr)) {
		condlog(0, "%s: devmap event #%i dm_task_set_event_nr error",
				waiter->mapname, waiter->event_nr);
		dm_task_destroy(waiter->dmt);
		waiter->dmt = NULL;
		return 1;
	}

	dm_task_no_open_count(waiter->dmt);

	/* wait */
	sigemptyset(&set);
	sigaddset(&set, SIGUSR2);
	pthread_sigmask(SIG_UNBLOCK, &set, &oldset);

	pthread_testcancel();
	r = dm_task_run(waiter->dmt);
	pthread_testcancel();

	pthread_sigmask(SIG_SETMASK, &oldset, NULL);
	dm_task_destroy(waiter->dmt);
	waiter->dmt = NULL;

	if (!r)	/* wait interrupted by signal */
		return -1;

	waiter->event_nr++;

	/*
	 * upon event ...
	 */
	while (1) {
		condlog(3, "%s: devmap event #%i",
				waiter->mapname, waiter->event_nr);

		/*
		 * event might be :
		 *
		 * 1) a table reload, which means our mpp structure is
		 *    obsolete : refresh it through update_multipath()
		 * 2) a path failed by DM : mark as such through
		 *    update_multipath()
		 * 3) map has gone away : stop the thread.
		 * 4) a path reinstate : nothing to do
		 * 5) a switch group : nothing to do
		 */
		pthread_cleanup_push(cleanup_lock, &waiter->vecs->lock);
		lock(waiter->vecs->lock);
		pthread_testcancel();
		r = update_multipath(waiter->vecs, waiter->mapname, 1);
		lock_cleanup_pop(waiter->vecs->lock);

		if (r) {
			condlog(2, "%s: event checker exit",
				waiter->mapname);
			return -1; /* stop the thread */
		}

		event_nr = dm_geteventnr(waiter->mapname);

		if (waiter->event_nr == event_nr)
			return 1; /* upon problem reschedule 1s later */

		waiter->event_nr = event_nr;
	}
	return -1; /* never reach there */
}
