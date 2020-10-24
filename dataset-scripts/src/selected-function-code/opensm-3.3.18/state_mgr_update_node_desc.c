static void state_mgr_update_node_desc(IN cl_map_item_t * obj, IN void *context)
{
	osm_madw_context_t mad_context;
	osm_node_t *p_node = (osm_node_t *) obj;
	osm_sm_t *sm = context;
	osm_physp_t *p_physp = NULL;
	unsigned i, num_ports;
	ib_api_status_t status;

	OSM_LOG_ENTER(sm->p_log);

	CL_ASSERT(p_node);

	OSM_LOG(sm->p_log, OSM_LOG_DEBUG,
		"Updating NodeDesc for 0x%016" PRIx64 "\n",
		cl_ntoh64(osm_node_get_node_guid(p_node)));

	/* get a physp to request from. */
	num_ports = osm_node_get_num_physp(p_node);
	for (i = 0; i < num_ports; i++)
		if ((p_physp = osm_node_get_physp_ptr(p_node, i)))
			break;

	if (!p_physp) {
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 331C: "
			"Failed to find any valid physical port object.\n");
		goto exit;
	}

	mad_context.nd_context.node_guid = osm_node_get_node_guid(p_node);

	status = osm_req_get(sm, osm_physp_get_dr_path_ptr(p_physp),
			     IB_MAD_ATTR_NODE_DESC, 0, FALSE,
			     ib_port_info_get_m_key(&p_physp->port_info),
			     CL_DISP_MSGID_NONE, &mad_context);
	if (status != IB_SUCCESS)
		OSM_LOG(sm->p_log, OSM_LOG_ERROR,
			"ERR 331B: Failure initiating NodeDescription request "
			"(%s) to 0x%016" PRIx64 "\n", ib_get_err_str(status),
			cl_ntoh64(osm_node_get_node_guid(p_node)));

exit:
	OSM_LOG_EXIT(sm->p_log);
}