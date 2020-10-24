ib_api_status_t osm_sm_state_mgr_check_legality(osm_sm_t * sm,
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

	OSM_LOG(sm->p_log, OSM_LOG_DEBUG, "Received signal %s in state %s\n",
		osm_get_sm_mgr_signal_str(signal),
		osm_get_sm_mgr_state_str(sm->p_subn->sm_state));

	switch (sm->p_subn->sm_state) {
	case IB_SMINFO_STATE_DISCOVERING:
		switch (signal) {
		case OSM_SM_SIGNAL_DISCOVERY_COMPLETED:
		case OSM_SM_SIGNAL_MASTER_OR_HIGHER_SM_DETECTED:
		case OSM_SM_SIGNAL_HANDOVER:
			status = IB_SUCCESS;
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
		case OSM_SM_SIGNAL_DISABLE:
		case OSM_SM_SIGNAL_HANDOVER:
		case OSM_SM_SIGNAL_ACKNOWLEDGE:
			status = IB_SUCCESS;
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
			status = IB_SUCCESS;
			break;
		default:
			sm_state_mgr_signal_error(sm, signal);
			status = IB_INVALID_PARAMETER;
			break;
		}
		break;

	case IB_SMINFO_STATE_MASTER:
		switch (signal) {
		case OSM_SM_SIGNAL_HANDOVER:
		case OSM_SM_SIGNAL_HANDOVER_SENT:
			status = IB_SUCCESS;
			break;
		default:
			sm_state_mgr_signal_error(sm, signal);
			status = IB_INVALID_PARAMETER;
			break;
		}
		break;

	default:
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 3209: "
			"Invalid state %s\n",
			osm_get_sm_mgr_state_str(sm->p_subn->sm_state));
		status = IB_INVALID_PARAMETER;

	}

	cl_spinlock_release(&sm->state_lock);

	OSM_LOG_EXIT(sm->p_log);
	return status;
}