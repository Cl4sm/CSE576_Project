		    void * trigger_data)
{
	my_uev_trigger = uev_trigger;
	my_trigger_data = trigger_data;

	mlockall(MCL_CURRENT | MCL_FUTURE);

	while (1) {
		LIST_HEAD(uevq_tmp);

		pthread_mutex_lock(uevq_lockp);
		servicing_uev = 0;
		/*
		 * Condition signals are unreliable,
		 * so make sure we only wait if we have to.
		 */
		if (list_empty(&uevq)) {
			pthread_cond_wait(uev_condp, uevq_lockp);
		}
		servicing_uev = 1;
		list_splice_init(&uevq, &uevq_tmp);
		pthread_mutex_unlock(uevq_lockp);
		if (!my_uev_trigger)
			break;
		service_uevq(&uevq_tmp);
	}
	condlog(3, "Terminating uev service queue");
	uevq_cleanup(&uevq);
	return 0;
}
