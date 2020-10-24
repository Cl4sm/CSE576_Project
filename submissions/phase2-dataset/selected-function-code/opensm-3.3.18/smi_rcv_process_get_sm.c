static void smi_rcv_process_get_sm(IN osm_sm_t * sm,
				   IN const osm_remote_sm_t * p_sm,
				   boolean_t light_sweep)
{
	const ib_sm_info_t *p_smi;

	OSM_LOG_ENTER(sm->p_log);

	p_smi = &p_sm->smi;

	OSM_LOG(sm->p_log, OSM_LOG_VERBOSE,
		"Detected SM 0x%016" PRIx64 " in state %u (%s)\n",
		cl_ntoh64(p_smi->guid), ib_sminfo_get_state(p_smi),
		osm_get_sm_mgr_state_str(ib_sminfo_get_state(p_smi)));

	/* Check the state of this SM vs. our own. */
	switch (sm->p_subn->sm_state) {
	case IB_SMINFO_STATE_NOTACTIVE:
		break;

	case IB_SMINFO_STATE_DISCOVERING:
		switch (ib_sminfo_get_state(p_smi)) {
		case IB_SMINFO_STATE_NOTACTIVE:
			break;
		case IB_SMINFO_STATE_MASTER:
			sm->master_sm_found = 1;
			/* save on the sm the guid of the current master. */
			OSM_LOG(sm->p_log, OSM_LOG_VERBOSE,
				"Found master SM. Updating sm_state_mgr master_guid: 0x%016"
				PRIx64 "\n", cl_ntoh64(p_smi->guid));
			sm->master_sm_guid = p_smi->guid;
			break;
		case IB_SMINFO_STATE_DISCOVERING:
		case IB_SMINFO_STATE_STANDBY:
			if (smi_rcv_remote_sm_is_higher(sm, p_smi)) {
				/* the remote is a higher sm - need to stop sweeping */
				sm->master_sm_found = 1;
				/* save on the sm the guid of the higher SM we found - */
				/* we will poll it - as long as it lives - we should be in Standby. */
				OSM_LOG(sm->p_log, OSM_LOG_VERBOSE,
					"Found higher SM. Updating sm_state_mgr master_guid:"
					" 0x%016" PRIx64 "\n",
					cl_ntoh64(p_smi->guid));
				sm->master_sm_guid = p_smi->guid;
			}
			break;
		default:
			break;
		}
		break;

	case IB_SMINFO_STATE_STANDBY:
		/* if the guid of the SM that sent us this response is equal to the */
		/* p_sm_mgr->master_guid - then this is a signal that the polling */
		switch (ib_sminfo_get_state(p_smi)) {
		case IB_SMINFO_STATE_MASTER:
			/* This means the master is alive */
			/* Signal that to the SM state mgr */
			osm_sm_state_mgr_signal_master_is_alive(sm);

			if (!smi_rcv_remote_sm_is_higher(sm, p_smi))
				osm_send_trap144(sm,
						 TRAP_144_MASK_SM_PRIORITY_CHANGE);
			break;
		case IB_SMINFO_STATE_STANDBY:
			/* This should be the response from the sm we are polling. */
			/* If it is - then signal master is alive */
			if (sm->master_sm_guid == p_sm->smi.guid) {
				/* Make sure that it is an SM with higher priority than us.
				   If we started polling it when it was master, and it moved
				   to standby - then it might be with a lower priority than
				   us - and then we don't want to continue polling it. */
				if (smi_rcv_remote_sm_is_higher(sm, p_smi))
					osm_sm_state_mgr_signal_master_is_alive
					    (sm);
			}
			break;
		default:
			/* any other state - do nothing */
			break;
		}
		break;

	case IB_SMINFO_STATE_MASTER:
		switch (ib_sminfo_get_state(p_smi)) {
		case IB_SMINFO_STATE_MASTER:
			/* If this is a response due to our polling, this means that we are
			 * waiting for a handover from this SM, and it is still alive -
			 * signal that. If we detected the remote SM with higher priority
			 * we should init a heavy sweep in order to go STANDBY. If we
			 * detected a remote SM with lower priority, we should resend trap144
			 * as it might not get it and we don't want to wait for a HANDOVER
			 * forever.
			 */
			if (sm->polling_sm_guid) {
				if (smi_rcv_remote_sm_is_higher(sm, p_smi))
					sm->p_subn->force_heavy_sweep = TRUE;
				else {
					/* Update master_sm_guid to the GUID of the newly
					 * found MASTER SM and send trap 144 to it.
					 */
					sm->master_sm_guid = sm->polling_sm_guid;
					osm_send_trap144(sm, TRAP_144_MASK_SM_PRIORITY_CHANGE);
				}
				osm_sm_state_mgr_signal_master_is_alive(sm);
			} else {
				/* This is a response we got while sweeping the subnet.
				 *
				 * If this is during a heavy sweep, we will handle a case of
				 * handover needed later on, when the sweep is done and all
				 * SMs are recognized.
				 *
				 * If this is during a light sweep, initiate a heavy sweep
				 * to initiate handover scenarios.
				 *
				 * Note that it does not matter if the remote SM is lower
				 * or higher priority.  If it is lower priority, we must
				 * wait for it HANDOVER.  If it is higher priority, we need
				 * to HANDOVER to it.  Both cases are handled after doing
				 * a heavy sweep.
				 */
				if (light_sweep)
					sm->p_subn->force_heavy_sweep = TRUE;
			}
			break;
		case IB_SMINFO_STATE_STANDBY:
			if (light_sweep &&
			    smi_rcv_remote_sm_is_higher(sm, p_smi))
				sm->p_subn->force_heavy_sweep = TRUE;
			break;
		default:
			/* any other state - do nothing */
			break;
		}
		break;

	default:
		break;
	}

	OSM_LOG_EXIT(sm->p_log);
}