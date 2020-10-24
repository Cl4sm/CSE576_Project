static void smi_rcv_process_get_request(IN osm_sm_t * sm,
					IN const osm_madw_t * p_madw)
{
	uint8_t payload[IB_SMP_DATA_SIZE];
	ib_sm_info_t *p_smi = (ib_sm_info_t *) payload;
	ib_api_status_t status;
	ib_sm_info_t *p_remote_smi;

	OSM_LOG_ENTER(sm->p_log);

	CL_ASSERT(p_madw);

	/* No real need to grab the lock for this function. */
	memset(payload, 0, sizeof(payload));

	CL_ASSERT(osm_madw_get_smp_ptr(p_madw)->method == IB_MAD_METHOD_GET);

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

	status = osm_resp_send(sm, p_madw, 0, payload);
	if (status != IB_SUCCESS) {
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 2F02: "
			"Error sending SMInfo response (%s)\n",
			ib_get_err_str(status));
		goto Exit;
	}

Exit:
	OSM_LOG_EXIT(sm->p_log);
}