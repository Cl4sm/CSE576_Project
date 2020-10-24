static void vl15_poller(IN void *p_ptr)
{
	ib_api_status_t status;
	osm_madw_t *p_madw;
	osm_vl15_t *p_vl = p_ptr;
	cl_qlist_t *p_fifo;
	int32_t max_smps = p_vl->max_wire_smps;
	int32_t max_smps2 = p_vl->max_wire_smps2;

	OSM_LOG_ENTER(p_vl->p_log);

	if (p_vl->thread_state == OSM_THREAD_STATE_NONE)
		p_vl->thread_state = OSM_THREAD_STATE_RUN;

	while (p_vl->thread_state == OSM_THREAD_STATE_RUN) {
		/*
		   Start servicing the FIFOs by pulling off MAD wrappers
		   and passing them to the transport interface.
		   There are lots of corner cases here so tread carefully.

		   The unicast FIFO has priority, since somebody is waiting
		   for a timely response.
		 */
		cl_spinlock_acquire(&p_vl->lock);

		if (cl_qlist_count(&p_vl->ufifo) != 0)
			p_fifo = &p_vl->ufifo;
		else
			p_fifo = &p_vl->rfifo;

		p_madw = (osm_madw_t *) cl_qlist_remove_head(p_fifo);

		cl_spinlock_release(&p_vl->lock);

		if (p_madw != (osm_madw_t *) cl_qlist_end(p_fifo)) {
			OSM_LOG(p_vl->p_log, OSM_LOG_DEBUG,
				"Servicing p_madw = %p\n", p_madw);
			if (OSM_LOG_IS_ACTIVE_V2(p_vl->p_log, OSM_LOG_FRAMES))
				osm_dump_dr_smp_v2(p_vl->p_log,
						   osm_madw_get_smp_ptr(p_madw),
						   FILE_ID, OSM_LOG_FRAMES);

			vl15_send_mad(p_vl, p_madw);
		} else
			/*
			   The VL15 FIFO is empty, so we have nothing left to do.
			 */
			status = cl_event_wait_on(&p_vl->signal,
						  EVENT_NO_TIMEOUT, TRUE);

		while (p_vl->p_stats->qp0_mads_outstanding_on_wire >= max_smps &&
		       p_vl->thread_state == OSM_THREAD_STATE_RUN) {
			status = cl_event_wait_on(&p_vl->signal,
						  p_vl->max_smps_timeout,
						  TRUE);
			if (status == CL_TIMEOUT) {
				if (max_smps < max_smps2)
					max_smps++;
				break;
			} else if (status != CL_SUCCESS) {
				OSM_LOG(p_vl->p_log, OSM_LOG_ERROR, "ERR 3E02: "
					"Event wait failed (%s)\n",
					CL_STATUS_MSG(status));
				break;
			}
			max_smps = p_vl->max_wire_smps;
		}
	}

	/*
	   since we abort immediately when the state != OSM_THREAD_STATE_RUN
	   we might have some mads on the queues. After the thread exits
	   the vl15 destroy routine should put these mads back...
	 */

	OSM_LOG_EXIT(p_vl->p_log);
}