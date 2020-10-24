int osm_lid_mgr_process_sm(IN osm_lid_mgr_t * p_mgr)
{
	int ret;

	OSM_LOG_ENTER(p_mgr->p_log);

	CL_ASSERT(p_mgr->p_subn->sm_port_guid);

	CL_PLOCK_EXCL_ACQUIRE(p_mgr->p_lock);

	/* initialize the port_lid_tbl and empty ranges list following the
	   persistent db */
	lid_mgr_init_sweep(p_mgr);

	ret = lid_mgr_process_our_sm_node(p_mgr);

	CL_PLOCK_RELEASE(p_mgr->p_lock);

	OSM_LOG_EXIT(p_mgr->p_log);
	return ret;
}