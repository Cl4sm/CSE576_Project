static void state_mgr_send_handover(IN osm_sm_t * sm, IN osm_remote_sm_t * p_sm)
{
	uint8_t payload[IB_SMP_DATA_SIZE];
	ib_sm_info_t *p_smi = (ib_sm_info_t *) payload;
	osm_madw_context_t context;
	const osm_port_t *p_port;
	ib_api_status_t status;

	OSM_LOG_ENTER(sm->p_log);

	/*
	 * Send a query of SubnSet(SMInfo) HANDOVER to the remote sm given.
	 */

	memset(&context, 0, sizeof(context));
	p_port = osm_get_port_by_guid(sm->p_subn, p_sm->smi.guid);
	if (p_port == NULL) {
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 3316: "
			"No port object on given remote_sm object\n");
		goto Exit;
	}

	/* update the master_guid in the sm_state_mgr object according to */
	/* the guid of the port where the new Master SM should reside. */
	OSM_LOG(sm->p_log, OSM_LOG_VERBOSE,
		"Handing over mastership. Updating sm_state_mgr master_guid: %016"
		PRIx64 " (node %s)\n", cl_ntoh64(p_port->guid),
		p_port->p_node ? p_port->p_node->print_desc : "UNKNOWN");
	sm->master_sm_guid = p_port->guid;

	context.smi_context.port_guid = p_port->guid;
	context.smi_context.set_method = TRUE;

	p_smi->guid = sm->p_subn->sm_port_guid;
	p_smi->act_count = cl_hton32(sm->p_subn->p_osm->stats.qp0_mads_sent);
	p_smi->pri_state = (uint8_t) (sm->p_subn->sm_state |
				      sm->p_subn->opt.sm_priority << 4);
	/*
	 * Return 0 for the SM key unless we authenticate the requester
	 * as the master SM.
	 */
	if (ib_sminfo_get_state(&p_sm->smi) == IB_SMINFO_STATE_MASTER) {
		OSM_LOG(sm->p_log, OSM_LOG_DEBUG,
			"Responding to master SM with real sm_key\n");
		p_smi->sm_key = sm->p_subn->opt.sm_key;
	} else {
		/* The requester is not authenticated as master - set sm_key to zero */
		OSM_LOG(sm->p_log, OSM_LOG_DEBUG,
			"Responding to SM not master with zero sm_key\n");
		p_smi->sm_key = 0;
	}

	CL_PLOCK_ACQUIRE(sm->p_lock);
	status = osm_req_set(sm, osm_physp_get_dr_path_ptr(p_port->p_physp),
			     payload, sizeof(payload), IB_MAD_ATTR_SM_INFO,
			     IB_SMINFO_ATTR_MOD_HANDOVER, FALSE,
			     ib_port_info_get_m_key(&p_port->p_physp->port_info),
			     CL_DISP_MSGID_NONE, &context);
	CL_PLOCK_RELEASE(sm->p_lock);

	if (status != IB_SUCCESS)
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 3317: "
			"Failure requesting SMInfo (%s), remote SM at 0x%"
			PRIx64 " (node %s)\n",
			ib_get_err_str(status), cl_ntoh64(p_port->guid),
			p_port->p_node ? p_port->p_node->print_desc : "UNKNOWN");

Exit:
	OSM_LOG_EXIT(sm->p_log);
}