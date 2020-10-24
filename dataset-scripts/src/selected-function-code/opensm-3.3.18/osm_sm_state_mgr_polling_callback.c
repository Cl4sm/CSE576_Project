void osm_sm_state_mgr_polling_callback(IN void *context)
{
	osm_sm_t *sm = context;
	uint32_t timeout;
	cl_status_t cl_status;
	uint8_t sm_state;

	OSM_LOG_ENTER(sm->p_log);

	cl_spinlock_acquire(&sm->state_lock);
	sm_state = sm->p_subn->sm_state;
	cl_spinlock_release(&sm->state_lock);

	CL_PLOCK_ACQUIRE(sm->p_lock);
	timeout = sm->p_subn->opt.sminfo_polling_timeout;

	/*
	 * We can be here in one of two cases:
	 * 1. We are a STANDBY sm polling on the master SM.
	 * 2. We are a MASTER sm, waiting for a handover from a remote master sm.
	 * If we are not in one of these cases - don't need to restart the poller.
	 */
	if (!((sm_state == IB_SMINFO_STATE_MASTER &&
	       sm->polling_sm_guid != 0) ||
	      sm_state == IB_SMINFO_STATE_STANDBY)) {
		CL_PLOCK_RELEASE(sm->p_lock);
		goto Exit;
	}

	/*
	 * If we are a STANDBY sm and the osm_exit_flag is set, then let's
	 * signal the subnet_up. This is relevant for the case of running only
	 * once. In that case - the program is stuck until this signal is
	 * received. In other cases - it is not relevant whether or not the
	 * signal is on - since we are currently in exit flow
	 */
	if (sm_state == IB_SMINFO_STATE_STANDBY && osm_exit_flag) {
		CL_PLOCK_RELEASE(sm->p_lock);
		OSM_LOG(sm->p_log, OSM_LOG_VERBOSE,
			"Signalling subnet_up_event\n");
		cl_event_signal(&sm->subnet_up_event);
		goto Exit;
	}

	/*
	 * If retry number reached the max_retry_number in the subnet opt - call
	 * osm_sm_state_mgr_process with signal OSM_SM_SIGNAL_POLLING_TIMEOUT
	 */
	OSM_LOG(sm->p_log, OSM_LOG_VERBOSE, "SM State %d (%s), Retry number:%d\n",
		sm->p_subn->sm_state,  osm_get_sm_mgr_state_str(sm->p_subn->sm_state),
		sm->retry_number);

	if (sm->retry_number > sm->p_subn->opt.polling_retry_number) {
		CL_PLOCK_RELEASE(sm->p_lock);
		OSM_LOG(sm->p_log, OSM_LOG_DEBUG,
			"Reached polling_retry_number value in retry_number. "
			"Go to DISCOVERY state\n");
		osm_sm_state_mgr_process(sm, OSM_SM_SIGNAL_POLLING_TIMEOUT);
		goto Exit;
	}

	/* Send a SubnGet(SMInfo) request to the remote sm (depends on our state) */
	if (sm_state_mgr_send_master_sm_info_req(sm, sm_state)) {
		/* Request sent, increment the retry number */
		sm->retry_number++;
	}

	CL_PLOCK_RELEASE(sm->p_lock);

	/* restart the timer */
	cl_status = cl_timer_start(&sm->polling_timer, timeout);
	if (cl_status != CL_SUCCESS)
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 3211: "
			"Failed to restart polling timer\n");

Exit:
	OSM_LOG_EXIT(sm->p_log);
}