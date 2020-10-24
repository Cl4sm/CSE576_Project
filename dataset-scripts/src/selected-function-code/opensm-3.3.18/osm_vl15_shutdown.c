void osm_vl15_shutdown(IN osm_vl15_t * p_vl, IN osm_mad_pool_t * p_mad_pool)
{
	osm_madw_t *p_madw;

	OSM_LOG_ENTER(p_vl->p_log);

	/* we only should get here after the VL15 interface was initialized */
	CL_ASSERT(p_vl->state == OSM_VL15_STATE_READY);

	/* grab a lock on the object */
	cl_spinlock_acquire(&p_vl->lock);

	/* go over all outstanding MADs and retire their transactions */

	/* first we handle the list of response MADs */
	p_madw = (osm_madw_t *) cl_qlist_remove_head(&p_vl->ufifo);
	while (p_madw != (osm_madw_t *) cl_qlist_end(&p_vl->ufifo)) {
		OSM_LOG(p_vl->p_log, OSM_LOG_DEBUG,
			"Releasing Response p_madw = %p\n", p_madw);

		osm_mad_pool_put(p_mad_pool, p_madw);

		p_madw = (osm_madw_t *) cl_qlist_remove_head(&p_vl->ufifo);
	}

	/* Request MADs we send out */
	p_madw = (osm_madw_t *) cl_qlist_remove_head(&p_vl->rfifo);
	while (p_madw != (osm_madw_t *) cl_qlist_end(&p_vl->rfifo)) {
		OSM_LOG(p_vl->p_log, OSM_LOG_DEBUG,
			"Releasing Request p_madw = %p\n", p_madw);

		osm_mad_pool_put(p_mad_pool, p_madw);
		osm_stats_dec_qp0_outstanding(p_vl->p_stats);

		p_madw = (osm_madw_t *) cl_qlist_remove_head(&p_vl->rfifo);
	}

	/* free the lock */
	cl_spinlock_release(&p_vl->lock);

	OSM_LOG_EXIT(p_vl->p_log);
}