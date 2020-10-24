void osm_vl15_destroy(IN osm_vl15_t * p_vl, IN struct osm_mad_pool *p_pool)
{
	osm_madw_t *p_madw;

	OSM_LOG_ENTER(p_vl->p_log);

	/*
	   Signal our threads that we're leaving.
	 */
	p_vl->thread_state = OSM_THREAD_STATE_EXIT;

	/*
	   Don't trigger unless event has been initialized.
	   Destroy the thread before we tear down the other objects.
	 */
	if (p_vl->state != OSM_VL15_STATE_INIT)
		cl_event_signal(&p_vl->signal);

	cl_thread_destroy(&p_vl->poller);

	/*
	   Return the outstanding messages to the pool
	 */

	cl_spinlock_acquire(&p_vl->lock);

	while (!cl_is_qlist_empty(&p_vl->rfifo)) {
		p_madw = (osm_madw_t *) cl_qlist_remove_head(&p_vl->rfifo);
		osm_mad_pool_put(p_pool, p_madw);
	}
	while (!cl_is_qlist_empty(&p_vl->ufifo)) {
		p_madw = (osm_madw_t *) cl_qlist_remove_head(&p_vl->ufifo);
		osm_mad_pool_put(p_pool, p_madw);
	}

	cl_spinlock_release(&p_vl->lock);

	cl_event_destroy(&p_vl->signal);
	p_vl->state = OSM_VL15_STATE_INIT;
	cl_spinlock_destroy(&p_vl->lock);

	OSM_LOG_EXIT(p_vl->p_log);
}