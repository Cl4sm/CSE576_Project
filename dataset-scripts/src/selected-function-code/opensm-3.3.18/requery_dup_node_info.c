static void requery_dup_node_info(IN osm_sm_t * sm, osm_physp_t * p_physp,
				  unsigned count)
{
	osm_madw_context_t context;
	osm_dr_path_t path;
	cl_status_t status;

	path = *osm_physp_get_dr_path_ptr(p_physp->p_remote_physp);
	if (osm_dr_path_extend(&path, p_physp->p_remote_physp->port_num)) {
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 0D08: "
			"DR path with hop count %d couldn't be extended\n",
			path.hop_count);
		return;
	}

	context.ni_context.node_guid =
	    p_physp->p_remote_physp->p_node->node_info.port_guid;
	context.ni_context.port_num = p_physp->p_remote_physp->port_num;
	context.ni_context.dup_node_guid = p_physp->p_node->node_info.node_guid;
	context.ni_context.dup_port_num = p_physp->port_num;
	context.ni_context.dup_count = count;

	status = osm_req_get(sm, &path, IB_MAD_ATTR_NODE_INFO, 0,
			     TRUE, 0, CL_DISP_MSGID_NONE, &context);

	if (status != IB_SUCCESS)
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 0D02: "
			"Failure initiating NodeInfo request (%s)\n",
			ib_get_err_str(status));
}