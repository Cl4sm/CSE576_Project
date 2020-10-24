void osm_vl15_post(IN osm_vl15_t * p_vl, IN osm_madw_t * p_madw)
{
	OSM_LOG_ENTER(p_vl->p_log);

	CL_ASSERT(p_vl->state == OSM_VL15_STATE_READY);

	OSM_LOG(p_vl->p_log, OSM_LOG_DEBUG, "Posting p_madw = %p\n", p_madw);

	/*
	   Determine in which fifo to place the pending madw.
	 */
	cl_spinlock_acquire(&p_vl->lock);
	if (p_madw->resp_expected == TRUE) {
		cl_qlist_insert_tail(&p_vl->rfifo, &p_madw->list_item);
		osm_stats_inc_qp0_outstanding(p_vl->p_stats);
	} else
		cl_qlist_insert_tail(&p_vl->ufifo, &p_madw->list_item);
	cl_spinlock_release(&p_vl->lock);

	OSM_LOG(p_vl->p_log, OSM_LOG_DEBUG,
		"%u QP0 MADs on wire, %u QP0 MADs outstanding\n",
		p_vl->p_stats->qp0_mads_outstanding_on_wire,
		p_vl->p_stats->qp0_mads_outstanding);

	osm_vl15_poll(p_vl);

	OSM_LOG_EXIT(p_vl->p_log);
}