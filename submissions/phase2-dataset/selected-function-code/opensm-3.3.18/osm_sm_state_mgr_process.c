ib_api_status_t osm_sm_state_mgr_process(osm_sm_t * sm,
					 IN osm_sm_signal_t signal)
{
	ib_api_status_t status = IB_SUCCESS;

	CL_ASSERT(sm);

	OSM_LOG_ENTER(sm->p_log);

	/*
	 * The state lock prevents many race conditions from screwing
	 * up the state transition process.
	 */
	cl_spinlock_acquire(&sm->state_lock);

	OSM_LOG(sm->p_log, OSM_LOG_DEBUG,
		"Received signal %s in state %s\n",
		osm_get_sm_mgr_signal_str(signal),
		osm_get_sm_mgr_state_str(sm->p_subn->sm_state));

	switch (sm->p_subn->sm_state) {
	case IB_SMINFO_STATE_DISCOVERING:
		switch (signal) {
		case OSM_SM_SIGNAL_DISCOVERY_COMPLETED:
			/*
			 * Update the state of the SM to MASTER
			 */
			/* Turn on the first_time_master_sweep flag */
			sm->p_subn->sm_state = IB_SMINFO_STATE_MASTER;
			osm_report_sm_state(sm);
			/*
			 * Make sure to set the subnet master_sm_base_lid
			 * to the sm_base_lid value
			 */
			CL_PLOCK_EXCL_ACQUIRE(sm->p_lock);
			sm->p_subn->first_time_master_sweep = TRUE;
			sm->p_subn->master_sm_base_lid =
			    sm->p_subn->sm_base_lid;
			CL_PLOCK_RELEASE(sm->p_lock);
			break;
		case OSM_SM_SIGNAL_MASTER_OR_HIGHER_SM_DETECTED:
			/*
			 * Finished all discovery actions - move to STANDBY
			 * start the polling
			 */
			sm->p_subn->sm_state = IB_SMINFO_STATE_STANDBY;
			osm_report_sm_state(sm);
			/*
			 * Since another SM is doing the LFT config - we should not
			 * ignore the results of it
			 */
			CL_PLOCK_EXCL_ACQUIRE(sm->p_lock);
			sm->p_subn->ignore_existing_lfts = FALSE;
			CL_PLOCK_RELEASE(sm->p_lock);
			sm_state_mgr_start_polling(sm);
			break;
		case OSM_SM_SIGNAL_HANDOVER:
			/*
			 * Signal for a new sweep. We need to discover the other SM.
			 * If we already discovered this SM, and got the
			 * HANDOVER - this means the remote SM is of lower priority.
			 * In this case we will stop polling it (since it is a lower
			 * priority SM in STANDBY state).
			 */
			osm_sm_signal(sm, OSM_SIGNAL_SWEEP);
			break;
		default:
			sm_state_mgr_signal_error(sm, signal);
			status = IB_INVALID_PARAMETER;
			break;
		}
		break;

	case IB_SMINFO_STATE_STANDBY:
		switch (signal) {
		case OSM_SM_SIGNAL_POLLING_TIMEOUT:
		case OSM_SM_SIGNAL_DISCOVER:
			/*
			 * case 1: Polling timeout occured - this means that the Master SM
			 * is no longer alive.
			 * case 2: Got a signal to move to DISCOVERING
			 * Move to DISCOVERING state and start sweeping
			 */
			sm->p_subn->sm_state = IB_SMINFO_STATE_DISCOVERING;
			osm_report_sm_state(sm);
			CL_PLOCK_EXCL_ACQUIRE(sm->p_lock);
			sm->p_subn->coming_out_of_standby = TRUE;
			CL_PLOCK_RELEASE(sm->p_lock);
			osm_sm_signal(sm, OSM_SIGNAL_SWEEP);
			break;
		case OSM_SM_SIGNAL_DISABLE:
			/*
			 * Update the state to NOT_ACTIVE
			 */
			sm->p_subn->sm_state = IB_SMINFO_STATE_NOTACTIVE;
			osm_report_sm_state(sm);
			break;
		case OSM_SM_SIGNAL_HANDOVER:
			/*
			 * Update the state to MASTER, and start sweeping
			 * OPTIONAL: send ACKNOWLEDGE
			 */
			/* Turn on the first_time_master_sweep flag */
			CL_PLOCK_EXCL_ACQUIRE(sm->p_lock);
			sm->p_subn->first_time_master_sweep = TRUE;
			/*
			 * Turn on the force_heavy_sweep - we want a
			 * heavy sweep to occur on the first sweep of this SM.
			 */
			sm->p_subn->force_heavy_sweep = TRUE;

			/*
			 * Make sure to set the subnet master_sm_base_lid
			 * to the sm_base_lid value
			 */
			sm->p_subn->master_sm_base_lid =
			    sm->p_subn->sm_base_lid;

			sm->p_subn->coming_out_of_standby = TRUE;

			CL_PLOCK_RELEASE(sm->p_lock);

			sm->p_subn->sm_state = IB_SMINFO_STATE_MASTER;
			osm_report_sm_state(sm);
			osm_sm_signal(sm, OSM_SIGNAL_SWEEP);
			break;
		case OSM_SM_SIGNAL_ACKNOWLEDGE:
			/*
			 * Do nothing - already moved to STANDBY
			 */
			break;
		default:
			sm_state_mgr_signal_error(sm, signal);
			status = IB_INVALID_PARAMETER;
			break;
		}
		break;

	case IB_SMINFO_STATE_NOTACTIVE:
		switch (signal) {
		case OSM_SM_SIGNAL_STANDBY:
			/*
			 * Update the state to STANDBY
			 * start the polling
			 */
			sm->p_subn->sm_state = IB_SMINFO_STATE_STANDBY;
			osm_report_sm_state(sm);
			sm_state_mgr_start_polling(sm);
			break;
		default:
			sm_state_mgr_signal_error(sm, signal);
			status = IB_INVALID_PARAMETER;
			break;
		}
		break;

	case IB_SMINFO_STATE_MASTER:
		switch (signal) {
		case OSM_SM_SIGNAL_POLLING_TIMEOUT:
			/*
			 * We received a polling timeout - this means that we
			 * waited for a remote master sm to send us a handover,
			 * but didn't get it, and didn't get a response from
			 * that remote sm.
			 * We want to force a heavy sweep - hopefully this
			 * occurred because the remote sm died, and we'll find
			 * this out and configure the subnet after a heavy sweep.
			 * We also want to clear the polling_sm_guid - since
			 * we are done polling on that remote sm - we are
			 * sweeping again.
			 */
		case OSM_SM_SIGNAL_HANDOVER:
			/*
			 * If we received a handover in a master state - then we
			 * want to force a heavy sweep. This means that either
			 * we are in a sweep currently - in this case - no
			 * change, or we are in idle state - since we
			 * recognized a master SM before - so we want to make a
			 * heavy sweep and reconfigure the new subnet.
			 * We also want to clear the polling_sm_guid - since
			 * we are done polling on that remote sm - we got a
			 * handover from it.
			 */
			OSM_LOG(sm->p_log, OSM_LOG_VERBOSE,
				"Forcing heavy sweep. Received signal %s\n",
				osm_get_sm_mgr_signal_str(signal));
			CL_PLOCK_EXCL_ACQUIRE(sm->p_lock);
			sm->polling_sm_guid = 0;
			sm->p_subn->first_time_master_sweep = TRUE;
			sm->p_subn->coming_out_of_standby = TRUE;
			sm->p_subn->force_heavy_sweep = TRUE;
			CL_PLOCK_RELEASE(sm->p_lock);
			osm_sm_signal(sm, OSM_SIGNAL_SWEEP);
			break;
		case OSM_SM_SIGNAL_HANDOVER_SENT:
			/*
			 * Just sent a HANDOVER signal - move to STANDBY
			 * start the polling
			 */
			sm->p_subn->sm_state = IB_SMINFO_STATE_STANDBY;
			osm_report_sm_state(sm);
			sm_state_mgr_start_polling(sm);
			break;
		case OSM_SM_SIGNAL_WAIT_FOR_HANDOVER:
			/*
			 * We found a remote master SM, and we are waiting for
			 * it to handover the mastership to us. Need to start
			 * polling that SM, to make sure it is alive, if it
			 * isn't - then we should move back to discovering,
			 * since something must have happened to it.
			 */
			sm_state_mgr_start_polling(sm);
			break;
		case OSM_SM_SIGNAL_DISCOVER:
			sm->p_subn->sm_state = IB_SMINFO_STATE_DISCOVERING;
			osm_report_sm_state(sm);
			break;
		default:
			sm_state_mgr_signal_error(sm, signal);
			status = IB_INVALID_PARAMETER;
			break;
		}
		break;

	default:
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 3208: "
			"Invalid state %s\n",
			osm_get_sm_mgr_state_str(sm->p_subn->sm_state));

	}

	cl_spinlock_release(&sm->state_lock);

	OSM_LOG_EXIT(sm->p_log);
	return status;
}