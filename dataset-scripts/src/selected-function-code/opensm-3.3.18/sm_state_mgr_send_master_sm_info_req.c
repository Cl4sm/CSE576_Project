static boolean_t sm_state_mgr_send_master_sm_info_req(osm_sm_t * sm, uint8_t sm_state)
{
	osm_madw_context_t context;
	const osm_port_t *p_port;
	ib_api_status_t status;
	osm_dr_path_t dr_path;
	ib_net64_t guid;
	boolean_t sent_req = FALSE;

	OSM_LOG_ENTER(sm->p_log);

	memset(&context, 0, sizeof(context));
	if (sm_state == IB_SMINFO_STATE_STANDBY) {
		/*
		 * We are in STANDBY state - this means we need to poll the
		 * master SM (according to master_guid).
		 * Send a query of SubnGet(SMInfo) to the subn
		 * master_sm_base_lid object.
		 */
		guid = sm->master_sm_guid;
	} else {
		/*
		 * We are not in STANDBY - this means we are in MASTER state -
		 * so we need to poll the SM that is saved in polling_sm_guid
		 * under sm.
		 * Send a query of SubnGet(SMInfo) to that SM.
		 */
		guid = sm->polling_sm_guid;
	}

	/* Verify that SM is not polling itself */
	if (guid == sm->p_subn->sm_port_guid) {
		OSM_LOG(sm->p_log, OSM_LOG_VERBOSE,
			"OpenSM doesn't poll itself\n");
		goto Exit;
	}

	p_port = osm_get_port_by_guid(sm->p_subn, guid);

	if (p_port == NULL) {
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 3203: "
			"No port object for GUID 0x%016" PRIx64 "\n",
			cl_ntoh64(guid));
		goto Exit;
	}

	context.smi_context.port_guid = guid;
	context.smi_context.set_method = FALSE;
	memcpy(&dr_path, osm_physp_get_dr_path_ptr(p_port->p_physp), sizeof(osm_dr_path_t));

	status = osm_req_get(sm, &dr_path,
			     IB_MAD_ATTR_SM_INFO, 0, FALSE,
			     ib_port_info_get_m_key(&p_port->p_physp->port_info),
			     CL_DISP_MSGID_NONE, &context);

	if (status != IB_SUCCESS)
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 3204: "
			"Failure requesting SMInfo (%s)\n",
			ib_get_err_str(status));
	else
		sent_req = TRUE;

Exit:
	OSM_LOG_EXIT(sm->p_log);

	return (sent_req);
}