void osm_vl15_poll(IN osm_vl15_t * p_vl)
{
	OSM_LOG_ENTER(p_vl->p_log);

	CL_ASSERT(p_vl->state == OSM_VL15_STATE_READY);

	/*
	   If we have room for more VL15 MADs on the wire,
	   then signal the poller thread.

	   This is not an airtight check, since the poller thread
	   could be just about to send another MAD as we signal
	   the event here.  To cover this rare case, the poller
	   thread checks for a spurious wake-up.
	 */
	if (p_vl->p_stats->qp0_mads_outstanding_on_wire <
	    (int32_t) p_vl->max_wire_smps) {
		OSM_LOG(p_vl->p_log, OSM_LOG_DEBUG,
			"Signalling poller thread\n");
		cl_event_signal(&p_vl->signal);
	}

	OSM_LOG_EXIT(p_vl->p_log);
}