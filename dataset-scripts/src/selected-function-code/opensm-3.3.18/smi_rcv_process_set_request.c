static void smi_rcv_process_set_request(IN osm_sm_t * sm,
					IN const osm_madw_t * p_madw)
{
	uint8_t payload[IB_SMP_DATA_SIZE];
	ib_smp_t *p_smp;
	ib_sm_info_t *p_smi = (ib_sm_info_t *) payload;
	ib_sm_info_t *sm_smi;
	ib_api_status_t status;
	osm_sm_signal_t sm_signal;
	ib_sm_info_t *p_remote_smi;

	OSM_LOG_ENTER(sm->p_log);

	CL_ASSERT(p_madw);

	memset(payload, 0, sizeof(payload));

	p_smp = osm_madw_get_smp_ptr(p_madw);
	sm_smi = ib_smp_get_payload_ptr(p_smp);

	if (p_smp->method != IB_MAD_METHOD_SET) {
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 2F03: "
			"Unsupported set method 0x%X\n", p_smp->method);
		goto Exit;
	}

	CL_PLOCK_ACQUIRE(sm->p_lock);

	p_smi->guid = sm->p_subn->sm_port_guid;
	p_smi->act_count = cl_hton32(sm->p_subn->p_osm->stats.qp0_mads_sent);
	p_smi->pri_state = (uint8_t) (sm->p_subn->sm_state |
				      sm->p_subn->opt.sm_priority << 4);
	/*
	   p.840 line 20 - Return 0 for the SM key unless we authenticate the
	   requester as the master SM.
	 */
	p_remote_smi = ib_smp_get_payload_ptr(osm_madw_get_smp_ptr(p_madw));
	if (ib_sminfo_get_state(p_remote_smi) == IB_SMINFO_STATE_MASTER) {
		OSM_LOG(sm->p_log, OSM_LOG_DEBUG,
			"Responding to master SM with real sm_key\n");
		p_smi->sm_key = sm->p_subn->opt.sm_key;
	} else {
		/* The requester is not authenticated as master - set sm_key to zero. */
		OSM_LOG(sm->p_log, OSM_LOG_DEBUG,
			"Responding to SM not master with zero sm_key\n");
		p_smi->sm_key = 0;
	}

	/* Check the legality of the packet */
	status = smi_rcv_check_set_req_legality(p_smp);
	if (status != IB_SUCCESS) {
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 2F04: "
			"Check legality failed. AttributeModifier:0x%X RemoteState:%s\n",
			p_smp->attr_mod,
			osm_get_sm_mgr_state_str(ib_sminfo_get_state(sm_smi)));
		status = osm_resp_send(sm, p_madw, 7, payload);
		if (status != IB_SUCCESS)
			OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 2F05: "
				"Error sending SMInfo response (%s)\n",
				ib_get_err_str(status));
		CL_PLOCK_RELEASE(sm->p_lock);
		goto Exit;
	}

	/* translate from IB_SMINFO_ATTR to OSM_SM_SIGNAL */
	switch (p_smp->attr_mod) {
	case IB_SMINFO_ATTR_MOD_HANDOVER:
		sm_signal = OSM_SM_SIGNAL_HANDOVER;
		break;
	case IB_SMINFO_ATTR_MOD_ACKNOWLEDGE:
		sm_signal = OSM_SM_SIGNAL_ACKNOWLEDGE;
		break;
	case IB_SMINFO_ATTR_MOD_DISABLE:
		sm_signal = OSM_SM_SIGNAL_DISABLE;
		break;
	case IB_SMINFO_ATTR_MOD_STANDBY:
		sm_signal = OSM_SM_SIGNAL_STANDBY;
		break;
	case IB_SMINFO_ATTR_MOD_DISCOVER:
		sm_signal = OSM_SM_SIGNAL_DISCOVER;
		break;
	default:
		/*
		   This code shouldn't be reached - checked in the
		   check legality
		 */
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 2F06: "
			"THIS CODE SHOULD NOT BE REACHED!!\n");
		CL_PLOCK_RELEASE(sm->p_lock);
		goto Exit;
	}

	CL_PLOCK_RELEASE(sm->p_lock);

	/* check legality of the needed transition in the SM state machine */
	status = osm_sm_state_mgr_check_legality(sm, sm_signal);
	if (status != IB_SUCCESS) {
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 2F07: "
			"Failed check of legality of needed SM transition. "
			"AttributeModifier:0x%X RemoteState:%s\n",
			p_smp->attr_mod,
			osm_get_sm_mgr_state_str(ib_sminfo_get_state(sm_smi)));
		status = osm_resp_send(sm, p_madw, 7, payload);
		if (status != IB_SUCCESS)
			OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 2F08: "
				"Error sending SMInfo response (%s)\n",
				ib_get_err_str(status));
		goto Exit;
	}

	/* the SubnSet(SMInfo) command is ok. Send a response. */
	status = osm_resp_send(sm, p_madw, 0, payload);
	if (status != IB_SUCCESS)
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 2F09: "
			"Error sending SMInfo response (%s)\n",
			ib_get_err_str(status));

	/* it is a legal packet - act according to it */

	/* if the AttributeModifier is STANDBY - need to save on the sm in */
	/* the master_sm_guid variable - the guid of the current master. */
	if (p_smp->attr_mod == IB_SMINFO_ATTR_MOD_STANDBY) {
		OSM_LOG(sm->p_log, OSM_LOG_VERBOSE,
			"Received a STANDBY signal. Updating "
			"sm_state_mgr master_guid: 0x%016" PRIx64 "\n",
			cl_ntoh64(sm_smi->guid));
		CL_PLOCK_EXCL_ACQUIRE(sm->p_lock);
		sm->master_sm_guid = sm_smi->guid;
		CL_PLOCK_RELEASE(sm->p_lock);
	}

	status = osm_sm_state_mgr_process(sm, sm_signal);

	if (status != IB_SUCCESS)
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 2F10: "
			"Error in SM state transition (%s)\n",
			ib_get_err_str(status));

Exit:
	OSM_LOG_EXIT(sm->p_log);
}