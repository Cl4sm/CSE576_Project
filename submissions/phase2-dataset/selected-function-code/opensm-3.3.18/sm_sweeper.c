static void sm_sweeper(IN void *p_ptr)
{
	ib_api_status_t status;
	osm_sm_t * p_sm = p_ptr;
	unsigned signals, i;

	OSM_LOG_ENTER(p_sm->p_log);

	while (p_sm->thread_state == OSM_THREAD_STATE_RUN) {
		/*
		 * Wait on the event with a timeout.
		 * Sweeps may be initiated "off schedule" by simply
		 * signaling the event.
		 */
		status = cl_event_wait_on(&p_sm->signal_event,
					  EVENT_NO_TIMEOUT, TRUE);

		if (status == CL_SUCCESS)
			OSM_LOG(p_sm->p_log, OSM_LOG_DEBUG,
				"Off schedule sweep signalled\n");
		else {
			OSM_LOG(p_sm->p_log, OSM_LOG_ERROR, "ERR 2E01: "
				"Event wait failed (%s)\n",
				CL_STATUS_MSG(status));
			continue;
		}

		if (osm_exit_flag)
			break;

		cl_spinlock_acquire(&p_sm->signal_lock);
		signals = p_sm->signal_mask;
		p_sm->signal_mask = 0;
		cl_spinlock_release(&p_sm->signal_lock);

		for (i = 0; signals; signals >>= 1, i++)
			if (signals & 1)
				sm_process(p_sm, i);
	}

	OSM_LOG_EXIT(p_sm->p_log);
}