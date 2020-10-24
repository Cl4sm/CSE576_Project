static ib_api_status_t state_mgr_sweep_hop_0(IN osm_sm_t * sm)
{
	ib_api_status_t status;
	osm_dr_path_t dr_path;
	osm_bind_handle_t h_bind;
	uint8_t path_array[IB_SUBNET_PATH_HOPS_MAX];

	OSM_LOG_ENTER(sm->p_log);

	memset(path_array, 0, sizeof(path_array));

	/*
	 * First, get the bind handle.
	 */
	h_bind = osm_sm_mad_ctrl_get_bind_handle(&sm->mad_ctrl);
	if (h_bind != OSM_BIND_INVALID_HANDLE) {
		OSM_LOG_MSG_BOX(sm->p_log, OSM_LOG_VERBOSE,
				"INITIATING HEAVY SWEEP");
		/*
		 * Start the sweep by clearing the port counts, then
		 * get our own NodeInfo at 0 hops.
		 */
		CL_PLOCK_ACQUIRE(sm->p_lock);

		cl_qmap_apply_func(&sm->p_subn->node_guid_tbl,
				   state_mgr_reset_node_count, sm);

		cl_qmap_apply_func(&sm->p_subn->port_guid_tbl,
				   state_mgr_reset_port_count, sm);

		cl_qmap_apply_func(&sm->p_subn->sw_guid_tbl,
				   state_mgr_reset_switch_count, sm);

		/* Set the in_sweep_hop_0 flag in subn to be TRUE.
		 * This will indicate the sweeping not to continue beyond the
		 * the current node.
		 * This is relevant for the case of SM on switch, since in the
		 * switch info we need to signal somehow not to continue
		 * the sweeping. */
		sm->p_subn->in_sweep_hop_0 = TRUE;

		CL_PLOCK_RELEASE(sm->p_lock);

		osm_dr_path_init(&dr_path, 0, path_array);
		CL_PLOCK_ACQUIRE(sm->p_lock);
		status = osm_req_get(sm, &dr_path, IB_MAD_ATTR_NODE_INFO, 0,
				     TRUE, 0, CL_DISP_MSGID_NONE, NULL);
		CL_PLOCK_RELEASE(sm->p_lock);
		if (status != IB_SUCCESS)
			OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 3305: "
				"Request for NodeInfo failed (%s)\n",
				ib_get_err_str(status));
	} else {
		OSM_LOG(sm->p_log, OSM_LOG_DEBUG,
			"No bound ports. Deferring sweep...\n");
		status = IB_INVALID_STATE;
	}

	OSM_LOG_EXIT(sm->p_log);
	return status;
}