static void cc_poller(void *p_ptr)
{
	osm_congestion_control_t *p_cc = p_ptr;
	osm_madw_t *p_madw;

	OSM_LOG_ENTER(p_cc->log);

	if (p_cc->thread_state == OSM_THREAD_STATE_NONE)
		p_cc->thread_state = OSM_THREAD_STATE_RUN;

	while (p_cc->thread_state == OSM_THREAD_STATE_RUN) {
		cl_spinlock_acquire(&p_cc->mad_queue_lock);

		p_madw = (osm_madw_t *) cl_qlist_remove_head(&p_cc->mad_queue);

		cl_spinlock_release(&p_cc->mad_queue_lock);

		if (p_madw != (osm_madw_t *) cl_qlist_end(&p_cc->mad_queue))
			cc_poller_send(p_cc, p_madw);
		else
			cl_event_wait_on(&p_cc->cc_poller_wakeup,
					 EVENT_NO_TIMEOUT, TRUE);
	}

	OSM_LOG_EXIT(p_cc->log);
}