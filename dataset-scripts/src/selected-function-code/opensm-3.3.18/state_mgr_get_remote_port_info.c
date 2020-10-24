static void state_mgr_get_remote_port_info(IN osm_sm_t * sm,
					   IN osm_physp_t * p_physp)
{
	osm_dr_path_t *p_dr_path;
	osm_dr_path_t rem_node_dr_path;
	osm_madw_context_t mad_context;
	ib_api_status_t status;

	OSM_LOG_ENTER(sm->p_log);

	/* generate a dr path leaving on the physp to the remote node */
	p_dr_path = osm_physp_get_dr_path_ptr(p_physp);
	memcpy(&rem_node_dr_path, p_dr_path, sizeof(osm_dr_path_t));
	if (osm_dr_path_extend(&rem_node_dr_path, osm_physp_get_port_num(p_physp))) {
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 332D: "
			"DR path with hop count %d couldn't be extended "
			"so skipping PortInfo query\n",
			p_dr_path->hop_count);
		goto Exit;
	}

	memset(&mad_context, 0, sizeof(mad_context));

	mad_context.pi_context.node_guid =
	    osm_node_get_node_guid(osm_physp_get_node_ptr(p_physp));
	mad_context.pi_context.port_guid = p_physp->port_guid;
	mad_context.pi_context.set_method = FALSE;
	mad_context.pi_context.light_sweep = TRUE;
	mad_context.pi_context.active_transition = FALSE;
	mad_context.pi_context.client_rereg = FALSE;

	/* note that with some negative logic - if the query failed it means
	 * that there is no point in going to heavy sweep */
	status = osm_req_get(sm, &rem_node_dr_path, IB_MAD_ATTR_PORT_INFO, 0,
			     TRUE, 0, CL_DISP_MSGID_NONE, &mad_context);
	if (status != IB_SUCCESS)
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 332E: "
			"Request for remote PortInfo from 0x%" PRIx64 " failed (%s)\n",
			cl_ntoh64(p_physp->port_guid), ib_get_err_str(status));

Exit:
	OSM_LOG_EXIT(sm->p_log);
}