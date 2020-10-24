static void sm_mad_ctrl_retire_trans_mad(IN osm_sm_mad_ctrl_t * p_ctrl,
					 IN osm_madw_t * p_madw)
{
	uint32_t outstanding;

	OSM_LOG_ENTER(p_ctrl->p_log);

	CL_ASSERT(p_madw);
	/*
	   Return the MAD & wrapper to the pool.
	 */
	OSM_LOG(p_ctrl->p_log, OSM_LOG_DEBUG,
		"Retiring MAD with TID 0x%" PRIx64 "\n",
		cl_ntoh64(osm_madw_get_smp_ptr(p_madw)->trans_id));

	osm_mad_pool_put(p_ctrl->p_mad_pool, p_madw);

	outstanding = osm_stats_dec_qp0_outstanding(p_ctrl->p_stats);

	OSM_LOG(p_ctrl->p_log, OSM_LOG_DEBUG, "%u QP0 MADs outstanding%s\n",
		p_ctrl->p_stats->qp0_mads_outstanding,
		outstanding ? "" : ": wire is clean.");

	OSM_LOG_EXIT(p_ctrl->p_log);
}