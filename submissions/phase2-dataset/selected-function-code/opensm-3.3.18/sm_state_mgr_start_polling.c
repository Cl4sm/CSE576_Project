static void sm_state_mgr_start_polling(osm_sm_t * sm)
{
	uint32_t timeout;
	cl_status_t cl_status;

	OSM_LOG_ENTER(sm->p_log);

	/*
	 * Init the retry_number back to zero - need to restart counting
	 */
	sm->retry_number = 0;

	/*
	 * Send a SubnGet(SMInfo) query to the current (or new) master found.
	 */
	CL_PLOCK_ACQUIRE(sm->p_lock);
	timeout = sm->p_subn->opt.sminfo_polling_timeout;
	sm_state_mgr_send_master_sm_info_req(sm, sm->p_subn->sm_state);
	CL_PLOCK_RELEASE(sm->p_lock);

	/*
	 * Start a timer that will wake up every sminfo_polling_timeout milliseconds.
	 * The callback of the timer will send a SubnGet(SMInfo) to the Master SM
	 * and restart the timer
	 */
	cl_status = cl_timer_start(&sm->polling_timer, timeout);
	if (cl_status != CL_SUCCESS)
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 3210: "
			"Failed to start polling timer\n");

	OSM_LOG_EXIT(sm->p_log);
}