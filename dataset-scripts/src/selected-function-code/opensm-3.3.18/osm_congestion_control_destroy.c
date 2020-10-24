void osm_congestion_control_destroy(osm_congestion_control_t * p_cc)
{
	osm_madw_t *p_madw;

	OSM_LOG_ENTER(p_cc->log);

	p_cc->thread_state = OSM_THREAD_STATE_EXIT;

	cl_event_signal(&p_cc->sig_mads_on_wire_continue);
	cl_event_signal(&p_cc->cc_poller_wakeup);

	cl_thread_destroy(&p_cc->cc_poller);

	cl_spinlock_acquire(&p_cc->mad_queue_lock);

	while (!cl_is_qlist_empty(&p_cc->mad_queue)) {
		p_madw = (osm_madw_t *) cl_qlist_remove_head(&p_cc->mad_queue);
		osm_mad_pool_put(p_cc->mad_pool, p_madw);
	}

	cl_spinlock_release(&p_cc->mad_queue_lock);

	cl_spinlock_destroy(&p_cc->mad_queue_lock);

	cl_event_destroy(&p_cc->cc_poller_wakeup);
	cl_event_destroy(&p_cc->outstanding_mads_done_event);
	cl_event_destroy(&p_cc->sig_mads_on_wire_continue);

	OSM_LOG_EXIT(p_cc->log);
}