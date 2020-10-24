static ib_api_status_t state_mgr_notify_lid_change(IN osm_sm_t * sm)
{
	ib_api_status_t status;
	osm_bind_handle_t h_bind;

	OSM_LOG_ENTER(sm->p_log);

	/*
	 * First, get the bind handle.
	 */
	h_bind = osm_sm_mad_ctrl_get_bind_handle(&sm->mad_ctrl);
	if (h_bind == OSM_BIND_INVALID_HANDLE) {
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 3306: "
			"No bound ports\n");
		status = IB_ERROR;
		goto Exit;
	}

	/*
	 * Notify the transport layer that we changed the local LID.
	 */
	status = osm_vendor_local_lid_change(h_bind);
	if (status != IB_SUCCESS)
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 3307: "
			"Vendor LID update failed (%s)\n",
			ib_get_err_str(status));

Exit:
	OSM_LOG_EXIT(sm->p_log);
	return status;
}