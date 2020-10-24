void osm_state_mgr_process(IN osm_sm_t * sm, IN osm_signal_t signal)
{
	CL_ASSERT(sm);

	OSM_LOG_ENTER(sm->p_log);

	OSM_LOG(sm->p_log, OSM_LOG_DEBUG, "Received signal %s in state %s\n",
		osm_get_sm_signal_str(signal),
		osm_get_sm_mgr_state_str(sm->p_subn->sm_state));

	switch (signal) {
	case OSM_SIGNAL_SWEEP:
		if (!sm->p_subn->sweeping_enabled) {
			OSM_LOG(sm->p_log, OSM_LOG_DEBUG, "sweeping disabled - "
				"ignoring signal %s in state %s\n",
				osm_get_sm_signal_str(signal),
				osm_get_sm_mgr_state_str(sm->p_subn->sm_state));
		} else
			do_sweep(sm);
		break;
	case OSM_SIGNAL_IDLE_TIME_PROCESS_REQUEST:
		do_process_mgrp_queue(sm);
		break;
	case OSM_SIGNAL_GUID_PROCESS_REQUEST:
		do_process_guid_queue(sm);
		break;
	default:
		CL_ASSERT(FALSE);
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 3320: "
			"Invalid SM signal %u\n", signal);
		break;
	}

	OSM_LOG_EXIT(sm->p_log);
}